#ifndef BUSINESS_MODEL_SETTING_VOLUMENECESSARY_H
#define BUSINESS_MODEL_SETTING_VOLUMENECESSARY_H
#include <quuid.h>
#include "Common/Enum/CommonEnum.h"
#include "VolumeNecessaryDetail.h"
#include <QSharedPointer>
#include <QMap>
#include <QDomNode>
#include "SettingDataBase.h"

namespace Business
{
	namespace Model
	{
		namespace Setting
		{
			class VolumeNecessary: public SettingDataBase
			{
			public:
				VolumeNecessary();
				~VolumeNecessary();
			private:
				QUuid  _id;
				Common::Enum::VolumeNecessaryOption::VolumeNecessaryOption _option;
				QMap<QUuid,QSharedPointer<VolumeNecessaryDetail>> _volumeNecessaryDetails;

			public:
				QSharedPointer<VolumeNecessaryDetail> getOrAddVolumeNecessaryDetail(const QUuid& id);
				QList<QSharedPointer<VolumeNecessaryDetail>> getVolumeNecessaryDetails() const;
				void addVolumeNecessaryDetail(const QUuid& id, const QSharedPointer<VolumeNecessaryDetail>& volumeNecessaryDetail);
				bool containsVolumeNecessaryDetail(const QUuid& id);
				void clearDetail();
				void update(const QDomNode& xmlVolumeNecessary);
                QSharedPointer<VolumeNecessary> copy();

			public:

				QUuid getId() const
				{
					return _id;
				}
				
				Common::Enum::VolumeNecessaryOption::VolumeNecessaryOption getOption() const
				{
					return _option;
				}


				void setId(const QUuid& value)
				{
					_id = value;
				}

				void setOption(const Common::Enum::VolumeNecessaryOption::VolumeNecessaryOption& value)
				{
					_option = value;
				}




			};
		}
	}
}
#endif
