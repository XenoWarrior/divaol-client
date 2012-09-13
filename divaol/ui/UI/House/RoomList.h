#pragma once

#include "ui/GUIChanEx/GUIChanListViewEx.h"
#include "ui/GUIChanEx/GUIChanListItemEx.h"
#include "ui/structure.h"

namespace diva
{
	namespace HouseUI
	{
		using namespace gcn;

		class RoomListItem : public ListItemEx
		{
		private:
			Image *image1, *image2, *image3;
			gcn::Rectangle r1, r2, r3;
			Network::RoomInfo info;
			static int x1, y1, x2, y2, x3, y3, x4, y4;

		public:

			RoomListItem();
			~RoomListItem();
			void setInfo(const Network::RoomInfo& info);
			void setLook(const std::wstring& f1, gcn::Rectangle r1, const std::wstring& f2, gcn::Rectangle r2, const std::wstring& f3, gcn::Rectangle r3);
			virtual void draw(Graphics* graphics, Font* font, int state, int alpha);

			static void setTextPosition(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
		};


		class RoomList : public ListViewEx
		{
		private:

		public:
			RoomList();
			~RoomList();
		};
	}
}