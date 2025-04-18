#include "stdafx.h"
#include "souistd.h"

#include "RichEditObjFactory.h"
#include "RichEditObj.h"
#include "RichEditOleCtrls.h"

namespace SOUI
{

    RichEditObjFactory::RichEditObjFactory()
    {
        _creaters[RichEditText::GetClassName()] = RichEditText::CreateObject;
        _creaters[RichEditBkElement::GetClassName()] = RichEditBkElement::CreateObject;
        _creaters[RichEditPara::GetClassName()] = RichEditPara::CreateObject;
        _creaters[RichEditContent::GetClassName()] = RichEditContent::CreateObject;
        _creaters[RichEditFetchMoreOle::GetClassName()] = RichEditFetchMoreOle::CreateObject;
        _creaters[RichEditImageOle::GetClassName()] = RichEditImageOle::CreateObject;
        _creaters[RichEditFileOle::GetClassName()] = RichEditFileOle::CreateObject;
        _creaters[RichEditSeparatorBar::GetClassName()] = RichEditSeparatorBar::CreateObject;
        _creaters[RichEditMetaFileOle::GetClassName()] = RichEditMetaFileOle::CreateObject;
        _creaters[RichEditReminderOle::GetClassName()] = RichEditReminderOle::CreateObject;
		_creaters[RichEditAudioOle::GetClassName()] = RichEditAudioOle::CreateObject;
		_creaters[RichEditVideoOle::GetClassName()] = RichEditVideoOle::CreateObject;
		_creaters[RichEditLocationOle::GetClassName()] = RichEditLocationOle::CreateObject;
		_creaters[RichEditLinkOle::GetClassName()] = RichEditLinkOle::CreateObject;
		_creaters[RichEditReEditorOle::GetClassName()] = RichEditReEditorOle::CreateObject;
    }

    RichEditObjFactory::~RichEditObjFactory()
    {
    }

} // namespace SOUI
