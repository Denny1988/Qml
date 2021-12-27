#ifndef BUSINESS_MODEL_COMPUTING_DEMANDCOMPUTING_H
#define BUSINESS_MODEL_COMPUTING_DEMANDCOMPUTING_H
#include <quuid.h>
#include <qdatetime.h>
#include <QMap>
#include <QDebug>
#include "ComputingResult.h"
#include <QMutexLocker>
#include <QThread>

namespace Business
{
	namespace Model
	{
		namespace Computing
		{
			class DemandComputing
			{
			protected:
				DemandComputing() : _mutex(QMutex::Recursive)
				{}
			private:
				static const int DemandTimeout = 2;

			private:
				uint lastDemandTime;
				QSharedPointer<DemandComputingResult> lastResult;
				QMutex _mutex;

			protected:
				QMap<QString, QSharedPointer<DemandComputing>> dependencies;
				virtual QSharedPointer<DemandComputingResult> doCompute()=0;


			private:
				bool isDemanded()
				{
					//qDebug() << "Last Time:" << QString::number(this->lastDemandTime) << "CurrentTime" <<  QString::number(QDateTime::currentDateTime().toTime_t());
					return (QDateTime::currentDateTime().toTime_t() - this->lastDemandTime) <= DemandTimeout;
				}

				QSharedPointer<DemandComputingResult> compute(bool isCommanded)
				{
					if(!this->dependencies.empty())
					{
						for (int i = 0; i < this->dependencies.count(); i++)
						{
							this->dependencies.values().at(i)->compute(isCommanded);
						}
					}

					if(isCommanded)
					{
						if(!this->isDemanded())
						{    
							this->lastResult = this->doCompute();
						}
						this->lastDemandTime = QDateTime::currentDateTime().toTime_t();						
					}
					else 
					{
						/*if(this->isDemanded())
						{*/
							this->lastResult = this->doCompute();
						/*}*/						
					}
					return this->lastResult;
				}

			public:
				QSharedPointer<DemandComputingResult> compute()
				{
					QMutexLocker locker(&this->_mutex);
					return this->compute(false);
				}

				void compute(QList<QUuid> dependencyKeys)
				{
					QMutexLocker locker(&this->_mutex);
					if(!this->dependencies.empty())
					{
						QMap<QString, QSharedPointer<DemandComputing>>::const_iterator item = this->dependencies.constBegin();
						while (item != this->dependencies.constEnd()) {
							if(dependencyKeys.contains(QUuid(item.key())))
								item.value()->compute(false);
							++item;
						}
					}
					this->lastResult = this->doCompute();
				}

				QSharedPointer<DemandComputingResult> getResult()
				{
					QMutexLocker locker(&this->_mutex);
					return this->compute(true);
				}

				QSharedPointer<DemandComputingResult> getLastResult()
				{
					return this->lastResult;
				}

				QMap<QString, QSharedPointer<DemandComputing>>& getDependency()
				{
					return this->dependencies;
				}

				void addDependency(QString id, QSharedPointer<DemandComputing> demandComputing)
				{
					QMutexLocker locker(&this->_mutex);
					this->dependencies.insert(id, demandComputing);
				}

				void removeDependency(QString id)
				{
					QMutexLocker locker(&this->_mutex);
					this->dependencies.remove(id);
				}
			};
		}
	}
}
#endif
