#pragma once
#include <string>
#include "Vectors.h"
#include <vector>
#include "GLDraw.h"

enum MenuElementType
{
	TEXT,
	SECTION,
	BUTTON,
	SLIDER
};

struct SliderCreationInfo {
	std::string text;
	float* pFloat;
	float vMin;
	float vMax;
	float vIncrement;
};

struct ButtonCreationInfo {
	std::string text;
	bool* pBoolean;
};

struct VisibleElement {
	std::string text;

};

struct ClickableElement : public VisibleElement {
	bool* pBoolean;

	bool GetEnabled() { return *pBoolean; }
	void SetEnabled(bool newStatus) { *pBoolean = newStatus; }
	void Toggle() { *pBoolean = !*pBoolean; }
};


struct TextCreationInfo {
	std::string text;
};

struct Element {
	Element* parent;
	int elementType;
	void* element;
	Element* child;

	static Element* CreateElement(void* data, int elementType);
	bool IsSection(){ return elementType == SECTION; }
};

struct Text : public VisibleElement {
	
	static Text* CreateText(void * data);
	void Render(int itmIndexY, int itmPaddingIdx);
};

template<typename T>
struct LinkedList {
	bool extinguishableLinkedList;
	std::vector<T*> linkedButtonList;
};

struct Button : public ClickableElement {
	LinkedList<Button> lnkedListInfo;
	
	void ToggleOffAllBtnsLnkedLst();
	Button* GetEnabledButtoninLnkedLst();
	void Render(int itmIndexY, int itmPaddingIdx);
	void RenderToggle(int itmIndexY);
	void HandleClick();
	static Button* CreateButton(void * data);
};



struct Slider : public VisibleElement {
	float* pFloat;
	float vMin;
	float vMax;
	float vIncrement;
	
	Slider(void* data);
	void Render(int itmIndexY, int itmPaddingIdx);
};

struct SectionCreationInfo {
	std::string sectionName;
	bool* pBoolean;
	bool bRoot;
};

struct Section : public ClickableElement {
	bool bRoot;
	std::vector<Element*> section_elements;
	Element* lastElementCreated;

	void	Render(int itmIndexY, int itmPaddingIndexX);
	void	RenderStatusArrow(int indexY);
	int	Update(int DrawIndexY, int itmPaddingIndexX);
	void   AddText(std::string sectionName);
	void AddButtom(std::string btnName, bool* pBoolean);
	void AddSlider(std::string _text, float* _pFloat, float _vMin, float _vMax, float _vIncrement = 0.3f);
	void UpdateLastElem(Element* newElem);

	Section* AddSection(std::string sectionName, bool* pBoolean, bool bRoot = false);
	Element* GetElementByID(int id) { return section_elements[id]; }
	Element* GetParentNormalized(Element* pElem);
	Element* GetChildNormalized(Element* pElem);
	void	AddLinkedButtonList(std::vector<ButtonCreationInfo> btns, bool extinguishable = true);
	void HandleClick();
	bool IsLast(Element* pElement);
	bool IsFirst(Element* pElement);


	Section(void* data);

};

enum Step {
	UP,
	DOWN
};

struct IndexInfo {
	Section* currSection;
	Element* currElement;
	int		entryWards;
};

class Menu
{
private:

public:
	Section* rootSection;
	Section* currSection;
	Element* currElement;
	bool	 currElementInited;
	Rect m_menu_bg_rect;
	float m_itmPadding;
	bool m_enabled;

	std::vector<IndexInfo> indexStack;

	Menu();
	void		Update();
	static void DrawString(char* str, Rect& r, const GLubyte color[3], float offsetX = 0.f);
	bool NeedNormalization(int goingWards);
	void DoStepNormalizated(int wards);
	void PushIndexContext(int goingWards);
	void MakeSectionCurrentIndexContext(Section* pSect, int startFrom);
	void PopIndexContext(int wards);
	void HandleStepUp();
	void HandleStepDown();
	void HandleContextDown();
	void HandleContextUp();
	void HandleSectionStepUp();
	void RenderBasicElements(std::string & name, float itmIndexX, float itmIndexY, void * element, const GLubyte textColor[3] = rgb::white);
	void InitCurrElement(Element* pElem);
	Rect GetItemRect(int yIndex);
	void HandleSectionEnter(bool fromStart = true);
	static float Perc2Res(float res, float perc);
};

extern Menu* menu;
extern Section* misc;

