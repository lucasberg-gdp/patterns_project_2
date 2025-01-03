#include <GLFW/glfw3.h>

class cInputHandler
{
public:
    static const unsigned int NUMBER_OF_KEYS = 512;
    bool m_Keys[NUMBER_OF_KEYS] = {};

    static const unsigned int NUMBER_OF_MODS = 8;
    bool m_Modes[NUMBER_OF_MODS] = {};

    const float OBJECT_MOVEMENT_SPEED = 1.0f;
    const float OBJECT_ROTATION_SPEED = 0.01f;
public:
    enum InputMode {
        GameMode,
        EditorMode
    };

    void KeyDown(unsigned int key);
    void KeyUp(unsigned int key);
    
    void ModeDown(unsigned char);
    void ModeUp(unsigned char);

    InputMode currentMode = InputMode::EditorMode;
    bool hasJustChangedInputMode = false;

    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void HandleInput() const;

    void OnPress(int key);
    void WhilePressed(int key) const;
    void OnRelease(int key);

    void SetObjectMovementSpeed(float objectSpeed);
    void SetObjectRotationSpeed(float objectRotationSpeed);

    bool AreAllModifiersUp(GLFWwindow* window);

    // mouse
    void AsyncMouseHandlingUpdate(GLFWwindow* window);
    void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    void MouseScrollWheelCallback(GLFWwindow* window, double xoffset, double yoffset);
    void MouseEnteredWindowCallback(GLFWwindow* window, int entered);
};