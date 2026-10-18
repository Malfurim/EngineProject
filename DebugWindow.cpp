#include "DebugWindow.h"

// --- ADDITIONAL INCLUDES ---
#include "Settings.h"
#include "ResourceManager.h"
#include "UIText.h"

// --- MACROS & DEFINES ---


// --- FORWARD DECLARATIONS ---


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC GLOBAL STATES & DATA								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
DebugWindow* DebugWindow::ms_instance = nullptr;

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CONSTRUCTORS & DESTRUCTOR								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
DebugWindow::DebugWindow() : UIWindow({ 0, 0 }, {SCREEN_WIDTH, SCREEN_HEIGHT}, L"", false, false, WINDOWSTYLE(L"empty"), WindowDrag::DRAG_NONE)
{
	Visibility = false;
	m_interactive = false;
	SetBackgroundColor({ 0.0f, 0.0f, 0.0f, 0.65f });
	EnsureTextBlocks();
}

DebugWindow::~DebugWindow()
{
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CORE FUNCTIONS											//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//bool DebugWindow::Initialize()
//{
//	return true;
//}

//void DebugWindow::Update()
//{
//}

//void DebugWindow::Render()
//{
//}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PUBLIC VIRTUAL FUNCTIONS								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	CLASS API												//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void DebugWindow::AddRow()
{
	m_textRows++;
	EnsureTextBlocks();
}

void DebugWindow::AddColumn()
{
	m_textColumns++;
	EnsureTextBlocks();
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	GETTERS & SETTERS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void DebugWindow::SetGridSize(unsigned int columns, unsigned int rows)
{
	if (rows < m_textRows || columns < m_textColumns)
		return;

	if (rows == m_textRows && columns == m_textColumns)
		return;

	m_textRows = rows;
	m_textColumns = columns;
	EnsureTextBlocks();
}

void DebugWindow::SetText(unsigned int column, unsigned int row, const std::wstring& text)
{
	if (!CheckVectorBounds(column, row))
		return;

	unsigned int index = row * m_textColumns + column;
	m_textBlocks[index]->SetText(text);
}

void DebugWindow::AddText(unsigned int column, unsigned int row, const std::wstring& text)
{
	if (!CheckVectorBounds(column, row))
		return;

	unsigned int index = row * m_textColumns + column;
	m_textBlocks[index]->SetText(m_textBlocks[index]->GetText() + text);
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	STATIC CLASS API										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
DebugWindow* DebugWindow::Instance()
{
	if (ms_instance == nullptr)
		ms_instance = new DebugWindow();
	return ms_instance;
}

// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED VIRTUAL FUNCTIONS								//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PROTECTED FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //


// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
//	PRIVATE FUNCTIONS										//
// ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** ** //
void DebugWindow::EnsureTextBlocks()
{
	if (m_textRows == m_prevRows && m_textColumns == m_prevColumns)
		return;

	float padding = 4.0f;
	Size size = { (SCREEN_WIDTH / m_textColumns) - padding * 2.0f,
				  (SCREEN_HEIGHT / m_textRows) - padding * 2.0f };
	Position blockPosition = { SCREEN_WIDTH / m_textColumns,
						       SCREEN_HEIGHT / m_textRows };

	unsigned int numBlocks = m_textRows * m_textColumns;
	std::vector<UIText*> newBlocks(numBlocks, nullptr);
	for (int i = 0; i < numBlocks; i++)
	{
		unsigned int row = i / m_textColumns;
		unsigned int column = i % m_textColumns;
		unsigned int oldVectorIndex = row * m_prevColumns + column;
		Position position = { blockPosition.X * column + padding, blockPosition.Y * row + padding};

		if (column < m_prevColumns && row < m_prevRows)
		{
			UIText* textBlock = m_textBlocks[oldVectorIndex];
			textBlock->SetPosition(position);
			textBlock->SetSize(size);

			newBlocks[i] = textBlock;
		}
		else 
		{
			UIText* textBlock = new UIText();
			textBlock->SetPosition(position);
			textBlock->SetSize(size);
			textBlock->SetFont(FONT(L"default"));
			textBlock->SetFontColor({ 1.0f, 1.0f, 1.0f, 1.0f });

			newBlocks[i] = textBlock;
			AddChild(textBlock);
		}
	}
	m_textBlocks = newBlocks;
	m_prevColumns = m_textColumns;
	m_prevRows = m_textRows;
}

bool DebugWindow::CheckVectorBounds(unsigned int column, unsigned int row)
{
	return (column < m_textColumns && row < m_textRows);
}
