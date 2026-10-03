#pragma once

// --- INCLUDES ---
#include "UIElement.h"
#include "Event.h"
#include <string>

// --- CONSTANTS ---
static constexpr unsigned int BUTTONSTATE_COUNT = 4;

// --- MACROS & DEFINES ---


// --- ENUMS & STRUCTS ---
enum ButtonState
{
	BUTTON_STATE_IDLE,
	BUTTON_STATE_HOVER,
	BUTTON_STATE_PRESSED,
	BUTTON_STATE_DISABLED,
};

// --- FORWARD DECLARATIONS ---
class UIImage;
class UIText;
class Font;
class Texture;

class UIButton : public UIElement
{
// - INTERFACE -
public:
	// --- CONSTRUCTORS & DESTRUCTOR ---
	UIButton();
	UIButton(Position position, Size size);
	~UIButton();

	// --- CORE FUNCTIONS ---
	// bool Initialize();
	void Update() override;
	void Render() override;

	// --- VIRTUAL FUNCTIONS ---


	// --- CLASS API ---
	void SetText(std::wstring text);
	void SetFont(Font* font);
	void SetFontSize(float size);
	void SetFontColor(Color color);
	void SetMaxLength(int length);

	void SetImage(Texture* texture);
	void SetImage(Texture* texture, ButtonState state);
	void SetBackgroundColor(Color color);
	void SetBackgroundColor(Color color, ButtonState state);
	void SetBorderColor(Color color);
	void SetBorderColor(Color color, ButtonState state);
	void SetBorderSize(float size);

	void EnableButton() { m_disabled = false; Invalidate(); }
	void DisableButton() { m_disabled = true; Invalidate();	}

	void EnableRepeatable() { m_repeatable = true; }
	void DisableRepeatable() { m_repeatable = false; }

	// --- GETTERS & SETTERS ---


	// --- STATIC CLASS API ---
	static bool IsCaptured() { return ms_captured != nullptr; }
	static bool IsCaptured(const UIButton* element) { return ms_captured == element; }
	static void ReleaseCapture()
	{
		if (IsCaptured())
		{
			ms_captured->LeftReleaseInternal(0.0f, 0.0f);
		}
	}

protected:
	// --- CONSTRUCTORS ---


	// --- VIRTUAL FUNCTIONS ---
	void LeftClickInternal(float mouseX, float mouseY) override;
	void LeftReleaseInternal(float mouseX, float mouseY) override;

	// --- PROTECTED FUNCTIONS ---


private:
	// --- CONSTRUCTORS ---


	// --- PRIVATE FUNCTIONS ---
	void UpdateButtonState();
	void EnsureImageObject();
	void EnsureTextObject();
	void OnPress();

// - PROPERTIES -
public:
	// --- PUBLIC COMPONENT STATES ---


	// --- PUBLIC COMPONENT DATA ---
	Event<UIButton*> OnClick{};

protected:
	// --- INTERNAL CONSTANTS ---


	// --- INTERNAL STRUCTS & ENUMS ---


	// --- INTERNAL COMPONENT STATES ---


	// --- INTERNAL COMPONENT DATA ---


private:
	// --- PRIVATE CONSTANTS ---
	static constexpr double PRESS_INTERVAL{ 0.10 };
	static constexpr double PRESS_DELAY{ 0.50 - PRESS_INTERVAL };

	// --- PRIVATE STRUCTS & ENUMS ---


	// --- PRIVATE COMPONENT STATES ---
	ButtonState m_buttonState{ ButtonState::BUTTON_STATE_IDLE };
	bool m_disabled{ false };
	bool m_colorChange{ false };
	bool m_repeatable{ false };
	double m_pressDelta{ 0.0 };
	double m_elapsedTime{ 0.0 };

	// --- PRIVATE COMPONENT DATA ---
	UIImage* m_image{ nullptr };
	UIText* m_text{ nullptr };

	Texture* m_texture[BUTTONSTATE_COUNT]{ nullptr };
	Color m_backgroundColor[BUTTONSTATE_COUNT]{};
	Color m_borderColor[BUTTONSTATE_COUNT]{};

	// --- STATIC GLOBAL STATES ---
	static UIButton* ms_captured;
};

