using UnityEngine;
using System.Collections;
using UnityEditor;

[CustomEditor(typeof(UnityLevel))]
public class UnityLevelEditor : Editor
{
    public override void OnInspectorGUI()
    {
        DrawDefaultInspector();

        UnityLevel myScript = (UnityLevel)target;
        if (GUILayout.Button("Load Scene"))
        {
            myScript.LoadCustomScene();
        }
        if (GUILayout.Button("Save Scene"))
        {
            myScript.SaveCustomScene();
        }
    }
}