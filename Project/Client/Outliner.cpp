#include "pch.h"
#include "Outliner.h"

#include "TreeUI.h"

Outliner::Outliner()
	: UI("Outliner", "##Outliner")
{
	//m_Tree = new TreeUI("OutlinerTree");
	//AddChildUI(m_Tree);
}

Outliner::~Outliner()
{
}

void Outliner::render_update()
{
}