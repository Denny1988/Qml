#ifndef FRAMEWORKENUM_H
#define FRAMEWORKENUM_H
#include <QMetaType>
namespace Common
{
	namespace Enum
	{
		namespace ColumnType
		{
			enum ColumnType
			{
				Normal = 1,
				StaticText = 2,
				CheckBox =3,
				NormalCheckBox = 4,
				AskBidAndPriceTrend = 5,
                ImageButton = 6,
				ColorSettingColumn = 7,
				AlignStaticColumn = 8,
				ColorColumn = 9,
				TimeAndPriceTrend = 10,
				InstrumentCodeColumn = 11,
				ColorSettingAndDblClickColumn = 12,
				AlignStaticAndDblClickColumn = 13,
				FIFOButton = 14,
				FastTradeButton = 15,
				DoubleSpinBox = 16,
                CurrencySelectButton = 17,
                OfficialFixingButton = 18
			};
		};

		namespace PriceDirection
		{
			enum  PriceDirection
			{
				PriceUp,
				PriceDown,
				NoChange,
			};
		};
		
		
		namespace TreeViewLevel
		{
			enum TreeViewLevel
			{
				Root,
				First,
				Second,
				Three
			};
		}

	}
}
Q_DECLARE_METATYPE(Common::Enum::PriceDirection::PriceDirection);
#endif
