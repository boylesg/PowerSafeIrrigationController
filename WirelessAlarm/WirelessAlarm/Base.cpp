#include "Control.h"
#include "Base.h"




CBase::CBase()
{
  m_eCtrlType = eNone;
}

CBase::~CBase()
{
}

bool CBase::isText()
{
  return m_eCtrlType == eText;
}

bool CBase::isCheckbox()
{
  return m_eCtrlType == eCheckBox;
}

bool CBase::isButton()
{
  return m_eCtrlType == eButton;
}

bool CBase::isKeyboard()
{
  return m_eCtrlType == eKeyboard;
}

bool CBase::isLabel()
{
  return m_eCtrlType == eLabel;
}

bool CBase::isDialog()
{
  return m_eCtrlType == eDialog;
}

bool CBase::isControl()
{
  return (m_eCtrlType == eLabel) || (m_eCtrlType == eKeyboard) || (m_eCtrlType == eButton) || (m_eCtrlType == eText);
}

void CBase::set(CBase &base)
{
  m_eCtrlType = base.m_eCtrlType;
}

CBase &CBase::operator =(CBase &base)
{
  set(base);
  return *this;
}


