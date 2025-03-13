#include <SFML/Graphics.hpp>
#include <vector>
#include "Settings.h"

using namespace std;
class BaseRunner : private sf::NonCopyable
{
public:
	static const sf::Time	TIME_PER_FRAME;
	static const int WINDOW_WIDTH = WINDOW_WIDTH;
	static const int WINDOW_HEIGHT = WINDOW_HEIGHT;

	BaseRunner();
	void run();

	static BaseRunner* getInstance();
	static BaseRunner* sharedInstance;
	float getFPS() const;
	sf::Time elapsed;

private:
	
	sf::RenderWindow		window;
	float fps = 0.0f;

	void render();
	void processEvents();
	void processInput(sf::Event::KeyEvent key);
	void update(sf::Time elapsedTime);
	
};


