#include "Menu.h"
#include <assert.h>

namespace ArkanoidGame
{
	void Menu::Init(const MenuItem& item, const sf::Font& menuFont)
	{
		font = &menuFont;
		
		rootItem.reset();  
		rootItem = std::make_unique<MenuItem>();

		SetupMenuItemFromSource(*rootItem, item, menuFont);	

		InitMenuItem(*rootItem);

		if (!rootItem->childrens.empty()) {
			SelectMenuItem(*rootItem->childrens.front());
		}
	}

	void Menu::SetupMenuItemFromSource(MenuItem& dest, const MenuItem& src, const sf::Font& font)
	{
		if (!dest.text) dest.text = std::make_unique<sf::Text>();
		if (!dest.hintText) dest.hintText = std::make_unique<sf::Text>();

		std::string textStr = src.text->getString().toAnsiString();
		std::string hintStr = src.hintText->getString().toAnsiString();

		dest.text->setFont(font);
		dest.text->setString(textStr);
		dest.text->setCharacterSize(24);
		dest.text->setFillColor(sf::Color::White);

		dest.hintText->setFont(font);
		dest.hintText->setString(hintStr);
		dest.hintText->setCharacterSize(24);
		dest.hintText->setFillColor(sf::Color::White);

		dest.childrenOrientation = src.childrenOrientation;
		dest.childrenAlignment = src.childrenAlignment;
		dest.childrenSpacing = src.childrenSpacing;
		dest.selectedColor = src.selectedColor;
		dest.deselectedColor = src.deselectedColor;
		dest.isEnabled = src.isEnabled;
		dest.onPressCallback = src.onPressCallback;
		dest.parent = nullptr;

		dest.childrens.clear();

		for (const auto& child : src.childrens)
		{
			auto newChild = std::make_unique<MenuItem>();
			SetupMenuItemFromSource(*newChild, *child, font); 
			dest.childrens.push_back(std::move(newChild));
		}
	}
	
	void Menu::InitMenuItem(MenuItem& item)
	{
		for (auto& child : item.childrens)
		{
			child->parent = &item;
			InitMenuItem(*child);
		}
	}

	void Menu::Draw(sf::RenderWindow& window, sf::Vector2f position, sf::Vector2f origin)
	{
		MenuItem& expandedItem = GetCurrentContext();

		std::vector<sf::Text*> texts;
		texts.reserve(expandedItem.childrens.size());
		for (auto& child : expandedItem.childrens) {
			if (child->isEnabled) {
				texts.push_back(child->text.get());
			}
		}

		DrawTextList(
			window,
			texts,
			expandedItem.childrenSpacing,
			expandedItem.childrenOrientation,
			expandedItem.childrenAlignment,
			position,
			origin);
	}
	
	void Menu::PressOnSelectedItem()
	{
		if (!selectedItem) {
			return;
		}

		if (selectedItem->onPressCallback) {
			selectedItem->onPressCallback(selectedItem, this);
			return;
		}

		// default behaviour
		if (!selectedItem->childrens.empty()) {
			SelectMenuItem(*selectedItem->childrens.front());
		}
	}

	void Menu::GoBack()
	{
		MenuItem& parent = GetCurrentContext();
		if (&parent != rootItem.get()) {
			SelectMenuItem(parent);
		}
	}

	void Menu::SwitchToPreviousMenuItem()
	{
		if (!selectedItem) {
			return;
		}
		
		MenuItem* parent = selectedItem->parent;
		assert(parent);

		auto it = std::find_if(parent->childrens.begin(), parent->childrens.end(), [this](const auto& item) {
			return selectedItem == item.get();
		});

		if (it != parent->childrens.begin()) {
			--it;
			SelectMenuItem(**it); 
		}
	}

	void Menu::SwitchToNextMenuItem()
	{
		if (!selectedItem) {
			return;
		}
		
		MenuItem* parent = selectedItem->parent;
		assert(parent);
		
		auto it = std::find_if(parent->childrens.begin(), parent->childrens.end(), [this](const auto& item) {
			return selectedItem == item.get();
			});
		it = std::next(it);
		if (it != parent->childrens.end()) {
			SelectMenuItem(*(*it));
		}
	}

	void Menu::SelectMenuItem(MenuItem& item)
	{
		assert(&item != rootItem.get());

		if (selectedItem == &item)
		{
			return;
		}

		if (!item.isEnabled)
		{
			// Don't allow to select disabled item
			return;
		}

		if (selectedItem)
		{
			selectedItem->text->setFillColor(selectedItem->deselectedColor);
		}

		selectedItem = &item;

		if (selectedItem)
		{
			selectedItem->text->setFillColor(selectedItem->selectedColor);
		}
	}	

	MenuItem& Menu::GetCurrentContext()
	{
		return selectedItem ? *(selectedItem->parent) : *rootItem;
	}
}
