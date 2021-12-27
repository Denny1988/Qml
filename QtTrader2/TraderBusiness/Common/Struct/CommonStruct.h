#ifndef COMMONSTRUCT_H
#define COMMONSTRUCT_H
#include <QUuid>

namespace Common
{
	namespace Struct
	{
		struct CardPoint
		{
			double  _x;
			double  _y;
		};

		struct AnswerForRecoverPassword
		{
		public:
			int     _sequence;
			QUuid   _questionId;
			QString _content;
		};
	
		struct QuestionForRecoverPassword
		{
		public:
			QUuid   _id;
			QString _content;
		};

		
		struct RecoverPasswordData
		{
			QList<QSharedPointer<QuestionForRecoverPassword>> _questions;
			QList<QSharedPointer<AnswerForRecoverPassword>> _answers;
		};

		struct ColumnStruct
		{
			int  _logicalIndex;
			int  _visualIndex;
			bool _isVisible;
			QString  _title;
			bool _isDisabled;
		};

		struct InstrumentStruct
		{
			public:
				QUuid     _Id;
				double   _left;
				double _top;
				bool _isShow;
		};
	}
}
#endif