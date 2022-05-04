#include "Menu.h"
#include <Windows.h>

extern WindowInfo* wi;
Menu* menu;

Menu::Menu()
{
	ZeroMemory(this, sizeof(Menu));
	m_menu_bg_rect.y		= wi->height * 0.02f;
	m_menu_bg_rect.width	= wi->width * 0.15f;
	m_menu_bg_rect.x = (wi->width - (wi->width * 0.01f)) - m_menu_bg_rect.width;
	m_itmPadding = (wi->height * 20.f) / 768.f;
	m_menu_bg_rect.height	= m_itmPadding;
	m_enabled				= true;

	
	currElementInited = false;

	SectionCreationInfo sci;
	sci.bRoot = true;
	sci.pBoolean = nullptr;
	rootSection = new Section(&sci);
	currSection = rootSection;
}

float offset = 0.0001f;

void Menu::Update()
{

	if (GetAsyncKeyState(VK_INSERT) & 1)
		m_enabled = !m_enabled;

	/*if (GetAsyncKeyState(VK_LEFT))
		offset -= 0.05f;

	if (GetAsyncKeyState(VK_RIGHT))
		offset += 0.05f;*/

	//printf("%.2f\n", offset);

	if (m_enabled)
	{
		if (GetAsyncKeyState(VK_UP) & 1)
		{
			HandleStepUp();
		}

		if (GetAsyncKeyState(VK_DOWN) & 1)
		{
			HandleStepDown();
		}

		if (GetAsyncKeyState(VK_LEFT) & 1)
		{
			switch (currElement->elementType)
			{

			case SLIDER:
			{
				auto* pSlider = (Slider*)currElement->element;
				float& rValue = *pSlider->pFloat;

				if (rValue - pSlider->vIncrement > pSlider->vMin)
					rValue -= pSlider->vIncrement;

			}
			break;
			}
		}

		if (GetAsyncKeyState(VK_RIGHT) & 1)
		{
			switch (currElement->elementType)
			{

			case SLIDER:
			{
				auto* pSlider = (Slider*)currElement->element;
				float& rValue = *pSlider->pFloat;

				if (rValue + pSlider->vIncrement < pSlider->vMax)
					rValue += pSlider->vIncrement;

			}
			break;
			}
		}

		if (GetAsyncKeyState(0x46) & 1)
		{
			switch (currElement->elementType)
			{

			case SECTION:
				((Section*)currElement->element)->HandleClick();
			break;

			case BUTTON:
				((Button*)currElement->element)->HandleClick();
			break;
			}
		}

		rootSection->Update(0, 0);
	}

	
}

void Menu::DrawString(char* str, Rect& r, const GLubyte color[3], float offsetX)
{
	textRender->DrawString(r.x + 3.f + offsetX, r.y + 13.f, color, str);
}

bool Menu::NeedNormalization(int goingWards)
{
	switch (goingWards)
	{
	case UP:
		return currElement->parent == nullptr;
		break;

	case DOWN:
		return currElement->child == nullptr;
		break;
	}

	return false;
}

Element * Element::CreateElement(void * data, int _elementType)
{
	Element* pElem = new Element();

	pElem->elementType = _elementType;

	switch (_elementType)
	{
	case SECTION:
	{
		Section* pSec = new Section(data);
		pElem->element = (void*)pSec;

	}
	break;

	case TEXT:
	{
		Text* pText = Text::CreateText(data);
		pElem->element = (void*)pText;

	}
	break;

	case BUTTON:
	{
		Button* pBtn = Button::CreateButton(data);
		pElem->element = (void*)pBtn;
	}
	break;

	case SLIDER:
	{
		Slider* pSlider = new Slider(data);
		pElem->element = (void*)pSlider;
	}
	break;

	}

	switch (pElem->elementType)
	{
	case SLIDER:
	case BUTTON:
	case SECTION:
		menu->InitCurrElement(pElem);
	break;
	};

	return pElem;
}

Text * Text::CreateText(void * data)
{
	TextCreationInfo* pTci = (TextCreationInfo*)data;
	Text* pSec = new Text();

	pSec->text = pTci->text;

	return pSec;
}

void Text::Render(int itmIndexY, int itmPaddingIndexX)
{
	menu->RenderBasicElements(text, float(itmPaddingIndexX), float(itmIndexY), this);
}

void Section::Render(int itmIndexY, int itmPaddingIndexX)
{
	menu->RenderBasicElements(text, float(itmPaddingIndexX), float(itmIndexY), this, rgb::yellow);
	RenderStatusArrow(itmIndexY);
}

void Section::RenderStatusArrow(int indexY)
{
	Rect StatusArrowRect = menu->GetItemRect(indexY);
	float perc = Menu::Perc2Res(menu->m_menu_bg_rect.width, 14.f);
	Vector3 center, a, b;
	center.x = StatusArrowRect.x + (StatusArrowRect.width - (perc / 2));
	center.y = StatusArrowRect.y + (StatusArrowRect.height / 2.f);

	bool bEnabled = GetEnabled();

	a = center;
	b = center;
	
	if (bEnabled)
	{
		a.x -= 5;
		b.x += 5;
		a.y = b.y = center.y - 5.f;
	}
	else {
		a.x = b.x = center.x - 5.f;
		a.y -= 5.f;
		b.y += 5.f;
	}
		

	GL::DrawLine(center.x, center.y, a.x, a.y, 1.7f, rgb::white);
	GL::DrawLine(center.x, center.y, b.x, b.y, 1.7f, rgb::white);
	
	
	
}

int Section::Update(int DrawIndexY, int itmPaddingIndexX)
{
	int totalRenderedStuff = 0;
	if (!bRoot)
	{
		Render(DrawIndexY, itmPaddingIndexX - 1);
		DrawIndexY++;
		if (!GetEnabled())
			return 0;
	}

	for (size_t i = 0; i < section_elements.size(); i++)
	{
		int currIndexY = DrawIndexY + totalRenderedStuff + i;
		switch (section_elements[i]->elementType)
		{
		case SECTION:
		{
			Section* pSec = (Section*)section_elements[i]->element;
			totalRenderedStuff += pSec->Update(currIndexY, itmPaddingIndexX + 1);
		}
		break;

		case TEXT:
		{
			Text* pText = (Text*)section_elements[i]->element;
			pText->Render(currIndexY, itmPaddingIndexX);
		}
		break;

		case BUTTON:
		{
			Button* pBtn = (Button*)section_elements[i]->element;
			pBtn->Render(currIndexY, itmPaddingIndexX);
		}
		break;

		case SLIDER:
		{
			Slider* pSlider = (Slider*)section_elements[i]->element;
			pSlider->Render(currIndexY, itmPaddingIndexX);
		}
		break;
		}
	}

	return totalRenderedStuff + section_elements.size();
}

void Section::AddText(std::string _text)
{
	TextCreationInfo tci;
	tci.text = _text;

	Element* pElem = Element::CreateElement(&tci, TEXT);
	UpdateLastElem(pElem);

	section_elements.push_back(pElem);
}

void Section::AddButtom(std::string btnName, bool * _pBoolean)
{
	ButtonCreationInfo bci;
	bci.text = btnName;
	bci.pBoolean = _pBoolean;

	Element* btnElement = Element::CreateElement(&bci, BUTTON);

	UpdateLastElem(btnElement);
	section_elements.push_back(btnElement);
}

void Section::AddSlider(std::string _text, float * _pFloat, float _vMin, float _vMax, float _vIncrement)
{
	SliderCreationInfo sci;
	sci.text = _text;
	sci.pFloat = _pFloat;
	sci.vMin = _vMin;
	sci.vMax = _vMax;
	sci.vIncrement = _vIncrement;

	Element* btnElement = Element::CreateElement(&sci, SLIDER);

	UpdateLastElem(btnElement);
	section_elements.push_back(btnElement);
}

void Section::UpdateLastElem(Element * newElem)
{
	newElem->parent = lastElementCreated;
	if (lastElementCreated)
		lastElementCreated->child = newElem;

	lastElementCreated = newElem;
}

Section * Section::AddSection(std::string _sectionName, bool * _pBoolean, bool _bRoot)
{
	SectionCreationInfo sci;
	sci.sectionName = _sectionName;
	sci.pBoolean = _pBoolean;
	sci.bRoot = _bRoot;

	Element* sectionElement = Element::CreateElement(&sci, SECTION);

	UpdateLastElem(sectionElement);
	section_elements.push_back(sectionElement);


	return (Section *)sectionElement->element;
}

Element * Section::GetParentNormalized(Element * pElem)
{
	Element* result;

	if (pElem->parent)
		result = pElem->parent;
	else
		result = section_elements[section_elements.size() - 1];

	return result;
}

Element * Section::GetChildNormalized(Element * pElem)
{
	Element* result;

	if (pElem->child)
		result = pElem->child;
	else
		result = section_elements[0];

	return result;
}

void Section::AddLinkedButtonList(std::vector<ButtonCreationInfo> btns, bool extinguishableLinkedList)
{

	std::vector<Button*> linkedButtonList;

	for (size_t i = 0; i < btns.size(); i++)
	{
		Element* currElmBtn = Element::CreateElement(&btns[i], BUTTON);
		Button* currBtn = (Button*)currElmBtn->element;

		UpdateLastElem(currElmBtn);
		section_elements.push_back(currElmBtn);
		linkedButtonList.push_back(currBtn);
	}

	for (size_t i = 0; i < linkedButtonList.size(); i++)
	{
		auto & currLinkedList = linkedButtonList[i]->lnkedListInfo;
		currLinkedList.extinguishableLinkedList = extinguishableLinkedList;
		currLinkedList.linkedButtonList = linkedButtonList;
	}

}

void Section::HandleClick()
{
	Toggle();
}

Section::Section(void * data)
{
	SectionCreationInfo* pSci = (SectionCreationInfo*)data;
	
	lastElementCreated = nullptr;
	text = pSci->sectionName;
	pBoolean = pSci->pBoolean;
	bRoot = pSci->bRoot;
}

void Menu::DoStepNormalizated(int goingWards)
{
	auto & pCurrSectElem = currSection->section_elements;
	switch (goingWards)
	{
	case UP:
	{
		if (NeedNormalization(goingWards))
			currElement = pCurrSectElem[pCurrSectElem.size() - 1];
		else
			currElement = currSection->GetParentNormalized(currElement);
	}
	break;
	case DOWN:
	{
		if (NeedNormalization(goingWards))
			currElement = pCurrSectElem[0];
		else
			currElement = currSection->GetChildNormalized(currElement);
	}
	break;
	}
}

void Menu::PushIndexContext(int goingWards)
{
	IndexInfo ii;
	ii.currSection = currSection;
	ii.currElement = currElement;
	ii.entryWards = (goingWards == UP) ? DOWN : UP;
	indexStack.push_back(ii);
}

void Menu::MakeSectionCurrentIndexContext(Section * pSect, int startFrom)
{
	currSection = pSect;
	auto & pCurrSectElem = currSection->section_elements;

	switch (startFrom)
	{
	case UP:
		currElement = pCurrSectElem[0];
		break;

	case DOWN:
		currElement = pCurrSectElem[pCurrSectElem.size() - 1];
		break;
	}
}

void Menu::PopIndexContext(int Outtrowards)
{
	IndexInfo ii = indexStack.back();

	currSection = ii.currSection;
	switch (Outtrowards)
	{
	case UP: // si se esta saliendo por arriba
	{
		if (ii.entryWards == DOWN)
			currElement = currSection->GetParentNormalized(ii.currElement);
		else
			currElement = ii.currElement;
	}
	break;

	case DOWN:// si se esta saliendo por abajo
	{
		if (ii.entryWards == UP)
			currElement = currSection->GetChildNormalized(ii.currElement);
		else
			currElement = ii.currElement;
	}
	break;
	}


	indexStack.pop_back();
}

void Menu::HandleStepUp()
{
	
	if (NeedNormalization(UP))
	{
		if (indexStack.size() != 0)
		{
			PopIndexContext(UP);
		}
		else {
			HandleSectionStepUp();
		}
	}
	else {
		HandleSectionStepUp();
	}
}

void Menu::HandleStepDown()
{

	if (currElement->IsSection())
	{
		auto* pSect = (Section*)currElement->element;
		if (pSect->GetEnabled())
		{
			PushIndexContext(DOWN);
			MakeSectionCurrentIndexContext(pSect, UP);
		}
		else {
			if (NeedNormalization(DOWN))
			{
				HandleContextDown();
			}
			else {
				DoStepNormalizated(DOWN);
			}
		}
	}
	else {
		if (NeedNormalization(DOWN))
		{
			HandleContextDown();
		}
		else {
			DoStepNormalizated(DOWN);
		}
	}
}

void Menu::HandleContextDown()
{
	if (indexStack.size() != 0)
	{
		PopIndexContext(DOWN);
		Element* parentElement = currSection->GetParentNormalized(currElement);
		if (parentElement->IsSection())
		{
			Section* pcurrElemSection = (Section*)parentElement->element;
			if (pcurrElemSection->GetEnabled())
			{
				if (parentElement->child == nullptr)
				{
					HandleContextDown();
				}
			}
		}
	}
	else {
		DoStepNormalizated(DOWN);
	}
}

void Menu::HandleContextUp()
{
	if (currElement->IsSection())
	{
		Section* currElementSection = (Section*)currElement->element;
		if (currElementSection->GetEnabled())
		{
			PushIndexContext(DOWN);
			MakeSectionCurrentIndexContext(currElementSection, DOWN);

			currElementSection = (Section*)currElement->element;
			if (currElement->IsSection())
			{
				if (currElementSection->GetEnabled())
				{
					HandleContextUp();
				}
			}
		}
	}
}

void Menu::HandleSectionStepUp()
{
	Element* parentNormalized = currSection->GetParentNormalized(currElement);
	if (parentNormalized->IsSection())
	{
		Section* pNormSect = (Section*)parentNormalized->element;
		if (pNormSect->GetEnabled())
		{
			PushIndexContext(UP);
			MakeSectionCurrentIndexContext(pNormSect, DOWN);
			HandleContextUp();
		}
		else {
			DoStepNormalizated(UP);
		}
	}
	else {
		DoStepNormalizated(UP);
	}
}

void Menu::RenderBasicElements(std::string & name, float itmIndexX, float itmIndexY, void * element, const GLubyte textColor[3])
{
	Rect currItmRect = menu->m_menu_bg_rect;
	currItmRect.y += itmIndexY * menu->m_itmPadding;
	auto color = (menu->currElementInited) ? ((void*)currElement->element == element) ? rgb::transpGray : rgb::darkGray : rgb::darkGray;
	GL::DrawRect(currItmRect, color);
	menu->DrawString((char*)name.c_str(), currItmRect, textColor, itmIndexX * 10.f);
}

void Menu::InitCurrElement(Element * pElem)
{
	if (!menu->currElementInited)
	{
		menu->currElement = pElem;
		menu->currElementInited = true;
	}
}

Rect Menu::GetItemRect(int yIndex)
{
	Rect rResult = menu->m_menu_bg_rect;
	rResult.y += yIndex * m_itmPadding;
	
	return rResult;
}

float Menu::Perc2Res(float res, float perc)
{
	return (perc * res) / 100;
}

Button * Button::CreateButton(void * data)
{
	ButtonCreationInfo* pBci = (ButtonCreationInfo*)data;
	Button* pButton = new Button();

	pButton->text = pBci->text;
	pButton->pBoolean = pBci->pBoolean;

	return pButton;
}

void Button::ToggleOffAllBtnsLnkedLst()
{
	auto & linkedButtonList = lnkedListInfo.linkedButtonList;
	for (size_t i = 0; i < linkedButtonList.size(); i++)
		*linkedButtonList[i]->pBoolean = false;
}

Button * Button::GetEnabledButtoninLnkedLst()
{
	auto & linkedButtonList = lnkedListInfo.linkedButtonList;
	for (size_t i = 0; i < linkedButtonList.size(); i++)
	{
		if (*linkedButtonList[i]->pBoolean == true)
			return linkedButtonList[i];
	}

	return nullptr;
}

void Button::Render(int itmIndexY, int itmPaddingIdx)
{
	menu->RenderBasicElements(text, float(itmPaddingIdx), float(itmIndexY), this);
	RenderToggle(itmIndexY);
	
}

void Button::RenderToggle(int itmIndexY)
{
	Rect toggleRect = menu->m_menu_bg_rect;

	float perc = Menu::Perc2Res(menu->m_menu_bg_rect.width, 14.f);
	toggleRect.x += toggleRect.width - perc;
	toggleRect.y += menu->m_itmPadding * itmIndexY;
	toggleRect.width = perc;
	GL::DrawRect(toggleRect, (*pBoolean) ? rgb::green : rgb::red);
}

void Button::HandleClick()
{
	auto & linkedButtonList = lnkedListInfo.linkedButtonList;
	if (linkedButtonList.size() != 0)
	{
		Button* currBtnSelected = GetEnabledButtoninLnkedLst();
		ToggleOffAllBtnsLnkedLst();

		if (this == currBtnSelected && lnkedListInfo.extinguishableLinkedList)
			SetEnabled(false);
		else
			SetEnabled(true);
	}
	else
		Toggle();
}

Slider::Slider(void* data)
{
	SliderCreationInfo* pSci = (SliderCreationInfo*)data;

	text = pSci->text;
	pFloat = pSci->pFloat;
	vMin = pSci->vMin;
	vMax = pSci->vMax;
	vIncrement = pSci->vIncrement;
}

void Slider::Render(int itmIndexY, int itmPaddingIdx)
{
	menu->RenderBasicElements(text, float(itmPaddingIdx), float(itmIndexY), this);

	Rect percRect = menu->GetItemRect(itmIndexY);
	Rect StrRect;
	
	float p = menu->Perc2Res(percRect.height, 15.f);
	
	StrRect.x = (percRect.x + percRect.width) - menu->Perc2Res(percRect.width,20.f);
	StrRect.y = percRect.y;

	percRect.y = (percRect.y + percRect.height) - p;
	percRect.height = p;

	percRect.width = vMin + ((*pFloat * percRect.width) / (abs(vMin) + abs(vMax)));
	
	GL::DrawRect(percRect, rgb::cyan);

	char buff[16];
	sprintf_s(buff, "%.1f", *pFloat);
	menu->DrawString(buff, StrRect, rgb::white);
}
