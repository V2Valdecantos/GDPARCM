using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Unity.VisualScripting;
using UnityEditor;
using UnityEngine;
using Object = UnityEngine.Object;

[Serializable]
public class SceneObject
{
    public string name;
    public Position position;
    public Rotation rotation;
    public Scale scale ;
    public string type ;
}

[Serializable]
public class Position
{
    public float x ;
    public float y ;
    public float z ;
}

[Serializable]
public class Rotation
{
    public float x ;
    public float y ;
    public float z ;
}

[Serializable]
public class Scale
{
    public float x ;
    public float y ;
    public float z ;
}

[Serializable]
public class SceneObjects
{
    public Dictionary<string, SceneObject> objects;
}

[Serializable]
public class UnityLevel : MonoBehaviour
{
    [SerializeField] private Object _asset;
    
    private List<GameObject> _newSceneRoots = new();
    [SerializeField] private Object _sceneToSave;

    public void LoadScene()
    {
        string path = AssetDatabase.GetAssetPath(_asset); ; // NEED TO GET THE PATH
        StreamReader reader = new StreamReader(path);
        string json = "";

        // Save the data into a Unity Level object.
        while (!reader.EndOfStream)
        {
            string line = reader.ReadLine();
            json += line;
        }

        reader.Close();

        ReadJson(json);
    }

    public void ReadJson(string json)
    {
        json = $"{{ \"objects\": {json} }}";
        SceneObjects objects = JsonUtility.FromJson<SceneObjects>(json);

        List<SceneObject> objectList = objects.objects.Values.ToList();
        //Debug.Log(objectList.Count);
        foreach (SceneObject obj in objectList)
        {
            CreateGameObjectFromSceneObject(obj);
        }
    }

    private void CreateGameObjectFromSceneObject(SceneObject sceneObject)
    {
        GameObject newObj = GameObject.CreatePrimitive(TypeIdToPrimitiveType(sceneObject.type));
        newObj.name = sceneObject.name;
        newObj.transform.position = new Vector3(sceneObject.position.x, sceneObject.position.y, sceneObject.position.z);
        newObj.transform.rotation = Quaternion.Euler(new Vector3(sceneObject.rotation.x, sceneObject.rotation.y, sceneObject.rotation.z) * Mathf.Rad2Deg);
        newObj.transform.localScale = new Vector3(sceneObject.scale.x, sceneObject.scale.y, sceneObject.scale.z);
    }

    private PrimitiveType TypeIdToPrimitiveType(string type)
    {
        switch (type)
        {
            case ".?AVCube@GDEngine@@":
                return PrimitiveType.Cube;
            case ".?AVPlane@GDEngine@@":
                return PrimitiveType.Plane;
            case ".?AVSphere@GDEngine@@":
                return PrimitiveType.Sphere;
            case ".?AVCylinder@GDEngine@@":
                return PrimitiveType.Cylinder;
            case ".?AVCapsule@GDEngine@@":
                return PrimitiveType.Capsule;
            default:
                break;
        }
        return PrimitiveType.Cube;
    }

    private void CreateTag(string tag)
    {
        // Open tag manager
        SerializedObject tagManager = new SerializedObject(AssetDatabase.LoadAllAssetsAtPath("ProjectSettings/TagManager.asset")[0]);
        SerializedProperty tagsProp = tagManager.FindProperty("tags");

        // First check if it is not already present
        bool found = false;
        for (int i = 0; i < tagsProp.arraySize; i++)
        {
            SerializedProperty t = tagsProp.GetArrayElementAtIndex(i);
            if (t.stringValue.Equals(tag)) { found = true; break; }
        }

        // if not found, add it
        if (!found)
        {
            tagsProp.InsertArrayElementAtIndex(0);
            SerializedProperty n = tagsProp.GetArrayElementAtIndex(0);
            n.stringValue = tag;
        }

        // and to save the changes
        tagManager.ApplyModifiedProperties();
    }

    public void LoadCustomScene()
    {
        string path = AssetDatabase.GetAssetPath(_asset); ; // NEED TO GET THE PATH
        StreamReader reader = new StreamReader(path);

        GameObject root = new GameObject("New Scene " + _newSceneRoots.Count);
        _newSceneRoots.Add(root);

        // Save the data into a Unity Level object.
        while (!reader.EndOfStream)
        {
            string line = reader.ReadLine();

            if (line == "{")
                line = reader.ReadLine();

            if (line == "Game Object")
            {

                // Name
                string name = reader.ReadLine();

               
                //Type
                string type = reader.ReadLine();
                PrimitiveType newObjType = TypeIdToPrimitiveType(type);
                GameObject newObj = GameObject.CreatePrimitive(newObjType);
                newObj.transform.parent = root.transform;
                newObj.name = name;
                CreateTag(type);
                newObj.tag = type;

                // Active
                line = reader.ReadLine();
                int isActive = 0;
                int.TryParse(line, out isActive);
                if (isActive == 0) newObj.SetActive(false);
                else newObj.SetActive(true);

                // Position (Format: x,y,z,)
                line = reader.ReadLine();
                Vector3 newObjPos = ReadVector3(line);

                // Scale
                line = reader.ReadLine();
                Vector3 newObjScale = ReadVector3(line);
                if (newObjType == PrimitiveType.Plane)
                {
                    newObjScale = new Vector3(newObjScale.x, newObjScale.z, newObjScale.y);
                }

                // Rotation
                line = reader.ReadLine();
                Vector3 newObjRot = ReadVector3(line) * Mathf.Rad2Deg;
                if (newObjType == PrimitiveType.Plane) newObjRot.x -= 90;
                Quaternion newObjQuat = Quaternion.Euler(newObjRot);

                // Set object transform
                newObj.transform.position = newObjPos;
                newObj.transform.localScale = newObjScale;
                newObj.transform.rotation = newObjQuat;

                // Rigidbody
                line = reader.ReadLine();
                if (line == "PhysicsComponent " + name)
                {
                    Rigidbody rb = newObj.AddComponent<Rigidbody>();

                    string rbType = reader.ReadLine();
                    if (rbType == "1") rb.isKinematic = true;
                    else rb.isKinematic = false;

                    if (newObjType == PrimitiveType.Quad && rb.isKinematic)
                        newObj.GetComponent<MeshCollider>().convex = true;
                }

                if (line != "}")
                    line = reader.ReadLine();
            }
        }

        reader.Close();
    }

    public Vector3 ReadVector3(string line)
    {
        Vector3 vec3 = Vector3.zero;

        string temp = "";
        int index = 0;
        foreach (char i in line)
        {
            if (i != ',')
                temp += i;
            else if (i == ',')
            {
                float num;
                float.TryParse(temp, out num);
                vec3[index] = num;

                index++;
                temp = "";
            }
        }

        return vec3;
    }

    private PrimitiveType StringToPrimitiveType(string type)
    {
        switch (type)
        {
            case "Cube":
                return PrimitiveType.Cube;
            case "Plane":
                return PrimitiveType.Plane;
            case "Sphere":
                return PrimitiveType.Sphere;
            case "Cylinder":
                return PrimitiveType.Cylinder;
            case "Capsule":
                return PrimitiveType.Capsule;
            default:
                break;
        }
        return PrimitiveType.Cube;
    }

    public void SaveCustomScene()
    {
        string path = AssetDatabase.GetAssetPath(_asset); // NEED TO GET THE PATH
        StreamWriter writer = File.CreateText("Assets/" + _sceneToSave.name + ".txt");

        foreach (Transform obj in _sceneToSave.GetComponentInChildren<Transform>())
        {
            GameObject go = obj.gameObject;
            
            writer.WriteLine("{");
            writer.WriteLine("Game Object");
            writer.WriteLine(obj.name);
            writer.WriteLine(obj.tag);
            writer.WriteLine((obj.gameObject.activeSelf) ? 1 : 0);
            writer.WriteLine(obj.transform.position.x.ToString() + "," + obj.transform.position.y.ToString() + "," + obj.transform.position.z.ToString() + ",");

            Vector3 scale = new Vector3(obj.transform.localScale.x, obj.transform.localScale.y, obj.transform.localScale.z);
            if (obj.tag.Contains("Plane")) scale = new Vector3(obj.transform.localScale.x, obj.transform.localScale.z, obj.transform.localScale.y);
            writer.WriteLine(scale.x.ToString() + "," + scale.y.ToString() + "," + scale.z.ToString() + ",");
            
            Vector3 rot = new Vector3(obj.transform.rotation.x, obj.transform.rotation.y, obj.transform.rotation.z) * Mathf.Deg2Rad;
            if (obj.tag.Contains("Plane")) rot.x += 1.57f;
            writer.WriteLine(rot.x.ToString() + "," + rot.y.ToString() + "," + rot.z.ToString() + ",");

            Rigidbody rb = obj.GetComponent<Rigidbody>();

            if (rb != null)
            {
                writer.WriteLine("PhysicsComponent " + obj.name);
                if (rb.isKinematic)
                    writer.WriteLine("0");
                else
                    writer.WriteLine("2");
            }
            writer.WriteLine("}");

        }

        writer.Close();
    }

    //private string PrimitiveTypeToString(PrimitiveType type)
    //{
    //    switch (type)
    //    {
    //        case PrimitiveType.Cube:
    //            return "Cube";
    //            break;
    //        case PrimitiveType.Plane:
    //            return "Plane";
    //            break;
    //        case PrimitiveType.Sphere:
    //            return "Sphere";
    //            break;
    //        case PrimitiveType.Cylinder:
    //            return "Cylinder";
    //            break;
    //        case PrimitiveType.Capsule:
    //            return "Capsule";
    //            break;
    //        default:
    //            break;
    //    }
    //    return "Cube";
    //}
}
