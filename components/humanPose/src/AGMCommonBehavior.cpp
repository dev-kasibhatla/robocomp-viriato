// **********************************************************************
//
// Copyright (c) 2003-2017 ZeroC, Inc. All rights reserved.
//
// This copy of Ice is licensed to you under the terms described in the
// ICE_LICENSE file included in this distribution.
//
// **********************************************************************
//
// Ice version 3.7.0
//
// <auto-generated>
//
// Generated from file `AGMCommonBehavior.ice'
//
// Warning: do not edit this file.
//
// </auto-generated>
//

#include <AGMCommonBehavior.h>
#include <IceUtil/PushDisableWarnings.h>
#include <Ice/LocalException.h>
#include <Ice/ValueFactory.h>
#include <Ice/OutgoingAsync.h>
#include <Ice/InputStream.h>
#include <Ice/OutputStream.h>
#include <IceUtil/PopDisableWarnings.h>

#if defined(_MSC_VER)
#   pragma warning(disable:4458) // declaration of ... hides class member
#elif defined(__clang__)
#   pragma clang diagnostic ignored "-Wshadow"
#elif defined(__GNUC__)
#   pragma GCC diagnostic ignored "-Wshadow"
#endif

#ifndef ICE_IGNORE_VERSION
#   if ICE_INT_VERSION / 100 != 307
#       error Ice version mismatch!
#   endif
#   if ICE_INT_VERSION % 100 > 50
#       error Beta header file detected
#   endif
#   if ICE_INT_VERSION % 100 < 0
#       error Ice patch level mismatch!
#   endif
#endif

#ifdef ICE_CPP11_MAPPING // C++11 mapping

namespace
{

const ::std::string iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_ids[2] =
{
    "::Ice::Object",
    "::RoboCompAGMCommonBehavior::AGMCommonBehavior"
};
const ::std::string iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_ops[] =
{
    "activateAgent",
    "deactivateAgent",
    "getAgentParameters",
    "getAgentState",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "killAgent",
    "reloadConfigAgent",
    "setAgentParameters",
    "uptimeAgent"
};
const ::std::string iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_activateAgent_name = "activateAgent";
const ::std::string iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_deactivateAgent_name = "deactivateAgent";
const ::std::string iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_getAgentState_name = "getAgentState";
const ::std::string iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_getAgentParameters_name = "getAgentParameters";
const ::std::string iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_setAgentParameters_name = "setAgentParameters";
const ::std::string iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_killAgent_name = "killAgent";
const ::std::string iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_uptimeAgent_name = "uptimeAgent";
const ::std::string iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_reloadConfigAgent_name = "reloadConfigAgent";

}

bool
RoboCompAGMCommonBehavior::AGMCommonBehavior::ice_isA(::std::string s, const ::Ice::Current&) const
{
    return ::std::binary_search(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_ids, iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_ids + 2, s);
}

::std::vector<::std::string>
RoboCompAGMCommonBehavior::AGMCommonBehavior::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector<::std::string>(&iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_ids[0], &iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_ids[2]);
}

::std::string
RoboCompAGMCommonBehavior::AGMCommonBehavior::ice_id(const ::Ice::Current&) const
{
    return ice_staticId();
}

const ::std::string&
RoboCompAGMCommonBehavior::AGMCommonBehavior::ice_staticId()
{
    static const ::std::string typeId = "::RoboCompAGMCommonBehavior::AGMCommonBehavior";
    return typeId;
}

bool
RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceD_activateAgent(::IceInternal::Incoming& inS, const ::Ice::Current& current)
{
    _iceCheckMode(::Ice::OperationMode::Normal, current.mode);
    auto istr = inS.startReadParams();
    ::RoboCompAGMCommonBehavior::ParameterMap iceP_prs;
    istr->readAll(iceP_prs);
    inS.endReadParams();
    bool ret = this->activateAgent(::std::move(iceP_prs), current);
    auto ostr = inS.startWriteParams();
    ostr->writeAll(ret);
    inS.endWriteParams();
    return true;
}

bool
RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceD_deactivateAgent(::IceInternal::Incoming& inS, const ::Ice::Current& current)
{
    _iceCheckMode(::Ice::OperationMode::Normal, current.mode);
    inS.readEmptyParams();
    bool ret = this->deactivateAgent(current);
    auto ostr = inS.startWriteParams();
    ostr->writeAll(ret);
    inS.endWriteParams();
    return true;
}

bool
RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceD_getAgentState(::IceInternal::Incoming& inS, const ::Ice::Current& current)
{
    _iceCheckMode(::Ice::OperationMode::Normal, current.mode);
    inS.readEmptyParams();
    ::RoboCompAGMCommonBehavior::StateStruct ret = this->getAgentState(current);
    auto ostr = inS.startWriteParams();
    ostr->writeAll(ret);
    inS.endWriteParams();
    return true;
}

bool
RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceD_getAgentParameters(::IceInternal::Incoming& inS, const ::Ice::Current& current)
{
    _iceCheckMode(::Ice::OperationMode::Normal, current.mode);
    inS.readEmptyParams();
    ::RoboCompAGMCommonBehavior::ParameterMap ret = this->getAgentParameters(current);
    auto ostr = inS.startWriteParams();
    ostr->writeAll(ret);
    inS.endWriteParams();
    return true;
}

bool
RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceD_setAgentParameters(::IceInternal::Incoming& inS, const ::Ice::Current& current)
{
    _iceCheckMode(::Ice::OperationMode::Normal, current.mode);
    auto istr = inS.startReadParams();
    ::RoboCompAGMCommonBehavior::ParameterMap iceP_prs;
    istr->readAll(iceP_prs);
    inS.endReadParams();
    bool ret = this->setAgentParameters(::std::move(iceP_prs), current);
    auto ostr = inS.startWriteParams();
    ostr->writeAll(ret);
    inS.endWriteParams();
    return true;
}

bool
RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceD_killAgent(::IceInternal::Incoming& inS, const ::Ice::Current& current)
{
    _iceCheckMode(::Ice::OperationMode::Normal, current.mode);
    inS.readEmptyParams();
    this->killAgent(current);
    inS.writeEmptyParams();
    return true;
}

bool
RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceD_uptimeAgent(::IceInternal::Incoming& inS, const ::Ice::Current& current)
{
    _iceCheckMode(::Ice::OperationMode::Normal, current.mode);
    inS.readEmptyParams();
    int ret = this->uptimeAgent(current);
    auto ostr = inS.startWriteParams();
    ostr->writeAll(ret);
    inS.endWriteParams();
    return true;
}

bool
RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceD_reloadConfigAgent(::IceInternal::Incoming& inS, const ::Ice::Current& current)
{
    _iceCheckMode(::Ice::OperationMode::Normal, current.mode);
    inS.readEmptyParams();
    bool ret = this->reloadConfigAgent(current);
    auto ostr = inS.startWriteParams();
    ostr->writeAll(ret);
    inS.endWriteParams();
    return true;
}

bool
RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceDispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair<const ::std::string*, const ::std::string*> r = ::std::equal_range(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_ops, iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_ops + 12, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_ops)
    {
        case 0:
        {
            return _iceD_activateAgent(in, current);
        }
        case 1:
        {
            return _iceD_deactivateAgent(in, current);
        }
        case 2:
        {
            return _iceD_getAgentParameters(in, current);
        }
        case 3:
        {
            return _iceD_getAgentState(in, current);
        }
        case 4:
        {
            return _iceD_ice_id(in, current);
        }
        case 5:
        {
            return _iceD_ice_ids(in, current);
        }
        case 6:
        {
            return _iceD_ice_isA(in, current);
        }
        case 7:
        {
            return _iceD_ice_ping(in, current);
        }
        case 8:
        {
            return _iceD_killAgent(in, current);
        }
        case 9:
        {
            return _iceD_reloadConfigAgent(in, current);
        }
        case 10:
        {
            return _iceD_setAgentParameters(in, current);
        }
        case 11:
        {
            return _iceD_uptimeAgent(in, current);
        }
        default:
        {
            assert(false);
            throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
        }
    }
}

void
RoboCompAGMCommonBehavior::AGMCommonBehaviorPrx::_iceI_activateAgent(const ::std::shared_ptr<::IceInternal::OutgoingAsyncT<bool>>& outAsync, const ::RoboCompAGMCommonBehavior::ParameterMap& iceP_prs, const ::Ice::Context& context)
{
    _checkTwowayOnly(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_activateAgent_name);
    outAsync->invoke(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_activateAgent_name, ::Ice::OperationMode::Normal, ::Ice::FormatType::DefaultFormat, context,
        [&](::Ice::OutputStream* ostr)
        {
            ostr->writeAll(iceP_prs);
        },
        nullptr);
}

void
RoboCompAGMCommonBehavior::AGMCommonBehaviorPrx::_iceI_deactivateAgent(const ::std::shared_ptr<::IceInternal::OutgoingAsyncT<bool>>& outAsync, const ::Ice::Context& context)
{
    _checkTwowayOnly(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_deactivateAgent_name);
    outAsync->invoke(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_deactivateAgent_name, ::Ice::OperationMode::Normal, ::Ice::FormatType::DefaultFormat, context,
        nullptr,
        nullptr);
}

void
RoboCompAGMCommonBehavior::AGMCommonBehaviorPrx::_iceI_getAgentState(const ::std::shared_ptr<::IceInternal::OutgoingAsyncT<::RoboCompAGMCommonBehavior::StateStruct>>& outAsync, const ::Ice::Context& context)
{
    _checkTwowayOnly(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_getAgentState_name);
    outAsync->invoke(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_getAgentState_name, ::Ice::OperationMode::Normal, ::Ice::FormatType::DefaultFormat, context,
        nullptr,
        nullptr);
}

void
RoboCompAGMCommonBehavior::AGMCommonBehaviorPrx::_iceI_getAgentParameters(const ::std::shared_ptr<::IceInternal::OutgoingAsyncT<::RoboCompAGMCommonBehavior::ParameterMap>>& outAsync, const ::Ice::Context& context)
{
    _checkTwowayOnly(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_getAgentParameters_name);
    outAsync->invoke(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_getAgentParameters_name, ::Ice::OperationMode::Normal, ::Ice::FormatType::DefaultFormat, context,
        nullptr,
        nullptr);
}

void
RoboCompAGMCommonBehavior::AGMCommonBehaviorPrx::_iceI_setAgentParameters(const ::std::shared_ptr<::IceInternal::OutgoingAsyncT<bool>>& outAsync, const ::RoboCompAGMCommonBehavior::ParameterMap& iceP_prs, const ::Ice::Context& context)
{
    _checkTwowayOnly(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_setAgentParameters_name);
    outAsync->invoke(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_setAgentParameters_name, ::Ice::OperationMode::Normal, ::Ice::FormatType::DefaultFormat, context,
        [&](::Ice::OutputStream* ostr)
        {
            ostr->writeAll(iceP_prs);
        },
        nullptr);
}

void
RoboCompAGMCommonBehavior::AGMCommonBehaviorPrx::_iceI_killAgent(const ::std::shared_ptr<::IceInternal::OutgoingAsyncT<void>>& outAsync, const ::Ice::Context& context)
{
    outAsync->invoke(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_killAgent_name, ::Ice::OperationMode::Normal, ::Ice::FormatType::DefaultFormat, context,
        nullptr,
        nullptr);
}

void
RoboCompAGMCommonBehavior::AGMCommonBehaviorPrx::_iceI_uptimeAgent(const ::std::shared_ptr<::IceInternal::OutgoingAsyncT<int>>& outAsync, const ::Ice::Context& context)
{
    _checkTwowayOnly(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_uptimeAgent_name);
    outAsync->invoke(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_uptimeAgent_name, ::Ice::OperationMode::Normal, ::Ice::FormatType::DefaultFormat, context,
        nullptr,
        nullptr);
}

void
RoboCompAGMCommonBehavior::AGMCommonBehaviorPrx::_iceI_reloadConfigAgent(const ::std::shared_ptr<::IceInternal::OutgoingAsyncT<bool>>& outAsync, const ::Ice::Context& context)
{
    _checkTwowayOnly(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_reloadConfigAgent_name);
    outAsync->invoke(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_reloadConfigAgent_name, ::Ice::OperationMode::Normal, ::Ice::FormatType::DefaultFormat, context,
        nullptr,
        nullptr);
}

::std::shared_ptr<::Ice::ObjectPrx>
RoboCompAGMCommonBehavior::AGMCommonBehaviorPrx::_newInstance() const
{
    return ::IceInternal::createProxy<AGMCommonBehaviorPrx>();
}

const ::std::string&
RoboCompAGMCommonBehavior::AGMCommonBehaviorPrx::ice_staticId()
{
    return RoboCompAGMCommonBehavior::AGMCommonBehavior::ice_staticId();
}

namespace Ice
{
}

#else // C++98 mapping

namespace
{

const ::std::string iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_activateAgent_name = "activateAgent";

const ::std::string iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_deactivateAgent_name = "deactivateAgent";

const ::std::string iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_getAgentState_name = "getAgentState";

const ::std::string iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_getAgentParameters_name = "getAgentParameters";

const ::std::string iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_setAgentParameters_name = "setAgentParameters";

const ::std::string iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_killAgent_name = "killAgent";

const ::std::string iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_uptimeAgent_name = "uptimeAgent";

const ::std::string iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_reloadConfigAgent_name = "reloadConfigAgent";

}
::IceProxy::Ice::Object* ::IceProxy::RoboCompAGMCommonBehavior::upCast(::IceProxy::RoboCompAGMCommonBehavior::AGMCommonBehavior* p) { return p; }

void
::IceProxy::RoboCompAGMCommonBehavior::_readProxy(::Ice::InputStream* istr, ::IceInternal::ProxyHandle< ::IceProxy::RoboCompAGMCommonBehavior::AGMCommonBehavior>& v)
{
    ::Ice::ObjectPrx proxy;
    istr->read(proxy);
    if(!proxy)
    {
        v = 0;
    }
    else
    {
        v = new ::IceProxy::RoboCompAGMCommonBehavior::AGMCommonBehavior;
        v->_copyFrom(proxy);
    }
}

::Ice::AsyncResultPtr
IceProxy::RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceI_begin_activateAgent(const ::RoboCompAGMCommonBehavior::ParameterMap& iceP_prs, const ::Ice::Context& context, const ::IceInternal::CallbackBasePtr& del, const ::Ice::LocalObjectPtr& cookie, bool sync)
{
    _checkTwowayOnly(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_activateAgent_name, sync);
    ::IceInternal::OutgoingAsyncPtr result = new ::IceInternal::CallbackOutgoing(this, iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_activateAgent_name, del, cookie, sync);
    try
    {
        result->prepare(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_activateAgent_name, ::Ice::Normal, context);
        ::Ice::OutputStream* ostr = result->startWriteParams(::Ice::DefaultFormat);
        ostr->write(iceP_prs);
        result->endWriteParams();
        result->invoke(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_activateAgent_name);
    }
    catch(const ::Ice::Exception& ex)
    {
        result->abort(ex);
    }
    return result;
}

bool
IceProxy::RoboCompAGMCommonBehavior::AGMCommonBehavior::end_activateAgent(const ::Ice::AsyncResultPtr& result)
{
    ::Ice::AsyncResult::_check(result, this, iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_activateAgent_name);
    bool ret;
    if(!result->_waitForResponse())
    {
        try
        {
            result->_throwUserException();
        }
        catch(const ::Ice::UserException& ex)
        {
            throw ::Ice::UnknownUserException(__FILE__, __LINE__, ex.ice_id());
        }
    }
    ::Ice::InputStream* istr = result->_startReadParams();
    istr->read(ret);
    result->_endReadParams();
    return ret;
}

::Ice::AsyncResultPtr
IceProxy::RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceI_begin_deactivateAgent(const ::Ice::Context& context, const ::IceInternal::CallbackBasePtr& del, const ::Ice::LocalObjectPtr& cookie, bool sync)
{
    _checkTwowayOnly(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_deactivateAgent_name, sync);
    ::IceInternal::OutgoingAsyncPtr result = new ::IceInternal::CallbackOutgoing(this, iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_deactivateAgent_name, del, cookie, sync);
    try
    {
        result->prepare(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_deactivateAgent_name, ::Ice::Normal, context);
        result->writeEmptyParams();
        result->invoke(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_deactivateAgent_name);
    }
    catch(const ::Ice::Exception& ex)
    {
        result->abort(ex);
    }
    return result;
}

bool
IceProxy::RoboCompAGMCommonBehavior::AGMCommonBehavior::end_deactivateAgent(const ::Ice::AsyncResultPtr& result)
{
    ::Ice::AsyncResult::_check(result, this, iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_deactivateAgent_name);
    bool ret;
    if(!result->_waitForResponse())
    {
        try
        {
            result->_throwUserException();
        }
        catch(const ::Ice::UserException& ex)
        {
            throw ::Ice::UnknownUserException(__FILE__, __LINE__, ex.ice_id());
        }
    }
    ::Ice::InputStream* istr = result->_startReadParams();
    istr->read(ret);
    result->_endReadParams();
    return ret;
}

::Ice::AsyncResultPtr
IceProxy::RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceI_begin_getAgentState(const ::Ice::Context& context, const ::IceInternal::CallbackBasePtr& del, const ::Ice::LocalObjectPtr& cookie, bool sync)
{
    _checkTwowayOnly(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_getAgentState_name, sync);
    ::IceInternal::OutgoingAsyncPtr result = new ::IceInternal::CallbackOutgoing(this, iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_getAgentState_name, del, cookie, sync);
    try
    {
        result->prepare(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_getAgentState_name, ::Ice::Normal, context);
        result->writeEmptyParams();
        result->invoke(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_getAgentState_name);
    }
    catch(const ::Ice::Exception& ex)
    {
        result->abort(ex);
    }
    return result;
}

::RoboCompAGMCommonBehavior::StateStruct
IceProxy::RoboCompAGMCommonBehavior::AGMCommonBehavior::end_getAgentState(const ::Ice::AsyncResultPtr& result)
{
    ::Ice::AsyncResult::_check(result, this, iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_getAgentState_name);
    ::RoboCompAGMCommonBehavior::StateStruct ret;
    if(!result->_waitForResponse())
    {
        try
        {
            result->_throwUserException();
        }
        catch(const ::Ice::UserException& ex)
        {
            throw ::Ice::UnknownUserException(__FILE__, __LINE__, ex.ice_id());
        }
    }
    ::Ice::InputStream* istr = result->_startReadParams();
    istr->read(ret);
    result->_endReadParams();
    return ret;
}

::Ice::AsyncResultPtr
IceProxy::RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceI_begin_getAgentParameters(const ::Ice::Context& context, const ::IceInternal::CallbackBasePtr& del, const ::Ice::LocalObjectPtr& cookie, bool sync)
{
    _checkTwowayOnly(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_getAgentParameters_name, sync);
    ::IceInternal::OutgoingAsyncPtr result = new ::IceInternal::CallbackOutgoing(this, iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_getAgentParameters_name, del, cookie, sync);
    try
    {
        result->prepare(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_getAgentParameters_name, ::Ice::Normal, context);
        result->writeEmptyParams();
        result->invoke(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_getAgentParameters_name);
    }
    catch(const ::Ice::Exception& ex)
    {
        result->abort(ex);
    }
    return result;
}

::RoboCompAGMCommonBehavior::ParameterMap
IceProxy::RoboCompAGMCommonBehavior::AGMCommonBehavior::end_getAgentParameters(const ::Ice::AsyncResultPtr& result)
{
    ::Ice::AsyncResult::_check(result, this, iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_getAgentParameters_name);
    ::RoboCompAGMCommonBehavior::ParameterMap ret;
    if(!result->_waitForResponse())
    {
        try
        {
            result->_throwUserException();
        }
        catch(const ::Ice::UserException& ex)
        {
            throw ::Ice::UnknownUserException(__FILE__, __LINE__, ex.ice_id());
        }
    }
    ::Ice::InputStream* istr = result->_startReadParams();
    istr->read(ret);
    result->_endReadParams();
    return ret;
}

::Ice::AsyncResultPtr
IceProxy::RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceI_begin_setAgentParameters(const ::RoboCompAGMCommonBehavior::ParameterMap& iceP_prs, const ::Ice::Context& context, const ::IceInternal::CallbackBasePtr& del, const ::Ice::LocalObjectPtr& cookie, bool sync)
{
    _checkTwowayOnly(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_setAgentParameters_name, sync);
    ::IceInternal::OutgoingAsyncPtr result = new ::IceInternal::CallbackOutgoing(this, iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_setAgentParameters_name, del, cookie, sync);
    try
    {
        result->prepare(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_setAgentParameters_name, ::Ice::Normal, context);
        ::Ice::OutputStream* ostr = result->startWriteParams(::Ice::DefaultFormat);
        ostr->write(iceP_prs);
        result->endWriteParams();
        result->invoke(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_setAgentParameters_name);
    }
    catch(const ::Ice::Exception& ex)
    {
        result->abort(ex);
    }
    return result;
}

bool
IceProxy::RoboCompAGMCommonBehavior::AGMCommonBehavior::end_setAgentParameters(const ::Ice::AsyncResultPtr& result)
{
    ::Ice::AsyncResult::_check(result, this, iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_setAgentParameters_name);
    bool ret;
    if(!result->_waitForResponse())
    {
        try
        {
            result->_throwUserException();
        }
        catch(const ::Ice::UserException& ex)
        {
            throw ::Ice::UnknownUserException(__FILE__, __LINE__, ex.ice_id());
        }
    }
    ::Ice::InputStream* istr = result->_startReadParams();
    istr->read(ret);
    result->_endReadParams();
    return ret;
}

::Ice::AsyncResultPtr
IceProxy::RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceI_begin_killAgent(const ::Ice::Context& context, const ::IceInternal::CallbackBasePtr& del, const ::Ice::LocalObjectPtr& cookie, bool sync)
{
    ::IceInternal::OutgoingAsyncPtr result = new ::IceInternal::CallbackOutgoing(this, iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_killAgent_name, del, cookie, sync);
    try
    {
        result->prepare(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_killAgent_name, ::Ice::Normal, context);
        result->writeEmptyParams();
        result->invoke(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_killAgent_name);
    }
    catch(const ::Ice::Exception& ex)
    {
        result->abort(ex);
    }
    return result;
}

void
IceProxy::RoboCompAGMCommonBehavior::AGMCommonBehavior::end_killAgent(const ::Ice::AsyncResultPtr& result)
{
    _end(result, iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_killAgent_name);
}

::Ice::AsyncResultPtr
IceProxy::RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceI_begin_uptimeAgent(const ::Ice::Context& context, const ::IceInternal::CallbackBasePtr& del, const ::Ice::LocalObjectPtr& cookie, bool sync)
{
    _checkTwowayOnly(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_uptimeAgent_name, sync);
    ::IceInternal::OutgoingAsyncPtr result = new ::IceInternal::CallbackOutgoing(this, iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_uptimeAgent_name, del, cookie, sync);
    try
    {
        result->prepare(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_uptimeAgent_name, ::Ice::Normal, context);
        result->writeEmptyParams();
        result->invoke(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_uptimeAgent_name);
    }
    catch(const ::Ice::Exception& ex)
    {
        result->abort(ex);
    }
    return result;
}

::Ice::Int
IceProxy::RoboCompAGMCommonBehavior::AGMCommonBehavior::end_uptimeAgent(const ::Ice::AsyncResultPtr& result)
{
    ::Ice::AsyncResult::_check(result, this, iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_uptimeAgent_name);
    ::Ice::Int ret;
    if(!result->_waitForResponse())
    {
        try
        {
            result->_throwUserException();
        }
        catch(const ::Ice::UserException& ex)
        {
            throw ::Ice::UnknownUserException(__FILE__, __LINE__, ex.ice_id());
        }
    }
    ::Ice::InputStream* istr = result->_startReadParams();
    istr->read(ret);
    result->_endReadParams();
    return ret;
}

::Ice::AsyncResultPtr
IceProxy::RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceI_begin_reloadConfigAgent(const ::Ice::Context& context, const ::IceInternal::CallbackBasePtr& del, const ::Ice::LocalObjectPtr& cookie, bool sync)
{
    _checkTwowayOnly(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_reloadConfigAgent_name, sync);
    ::IceInternal::OutgoingAsyncPtr result = new ::IceInternal::CallbackOutgoing(this, iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_reloadConfigAgent_name, del, cookie, sync);
    try
    {
        result->prepare(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_reloadConfigAgent_name, ::Ice::Normal, context);
        result->writeEmptyParams();
        result->invoke(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_reloadConfigAgent_name);
    }
    catch(const ::Ice::Exception& ex)
    {
        result->abort(ex);
    }
    return result;
}

bool
IceProxy::RoboCompAGMCommonBehavior::AGMCommonBehavior::end_reloadConfigAgent(const ::Ice::AsyncResultPtr& result)
{
    ::Ice::AsyncResult::_check(result, this, iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_reloadConfigAgent_name);
    bool ret;
    if(!result->_waitForResponse())
    {
        try
        {
            result->_throwUserException();
        }
        catch(const ::Ice::UserException& ex)
        {
            throw ::Ice::UnknownUserException(__FILE__, __LINE__, ex.ice_id());
        }
    }
    ::Ice::InputStream* istr = result->_startReadParams();
    istr->read(ret);
    result->_endReadParams();
    return ret;
}

::IceProxy::Ice::Object*
IceProxy::RoboCompAGMCommonBehavior::AGMCommonBehavior::_newInstance() const
{
    return new AGMCommonBehavior;
}

const ::std::string&
IceProxy::RoboCompAGMCommonBehavior::AGMCommonBehavior::ice_staticId()
{
    return ::RoboCompAGMCommonBehavior::AGMCommonBehavior::ice_staticId();
}

RoboCompAGMCommonBehavior::AGMCommonBehavior::~AGMCommonBehavior()
{
}

::Ice::Object* RoboCompAGMCommonBehavior::upCast(::RoboCompAGMCommonBehavior::AGMCommonBehavior* p) { return p; }


namespace
{
const ::std::string iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_ids[2] =
{
    "::Ice::Object",
    "::RoboCompAGMCommonBehavior::AGMCommonBehavior"
};

}

bool
RoboCompAGMCommonBehavior::AGMCommonBehavior::ice_isA(const ::std::string& s, const ::Ice::Current&) const
{
    return ::std::binary_search(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_ids, iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_ids + 2, s);
}

::std::vector< ::std::string>
RoboCompAGMCommonBehavior::AGMCommonBehavior::ice_ids(const ::Ice::Current&) const
{
    return ::std::vector< ::std::string>(&iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_ids[0], &iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_ids[2]);
}

const ::std::string&
RoboCompAGMCommonBehavior::AGMCommonBehavior::ice_id(const ::Ice::Current&) const
{
    return ice_staticId();
}

const ::std::string&
RoboCompAGMCommonBehavior::AGMCommonBehavior::ice_staticId()
{
#ifdef ICE_HAS_THREAD_SAFE_LOCAL_STATIC
    static const ::std::string typeId = "::RoboCompAGMCommonBehavior::AGMCommonBehavior";
    return typeId;
#else
    return iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_ids[1];
#endif
}

bool
RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceD_activateAgent(::IceInternal::Incoming& inS, const ::Ice::Current& current)
{
    _iceCheckMode(::Ice::Normal, current.mode);
    ::Ice::InputStream* istr = inS.startReadParams();
    ::RoboCompAGMCommonBehavior::ParameterMap iceP_prs;
    istr->read(iceP_prs);
    inS.endReadParams();
    bool ret = this->activateAgent(iceP_prs, current);
    ::Ice::OutputStream* ostr = inS.startWriteParams();
    ostr->write(ret);
    inS.endWriteParams();
    return true;
}

bool
RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceD_deactivateAgent(::IceInternal::Incoming& inS, const ::Ice::Current& current)
{
    _iceCheckMode(::Ice::Normal, current.mode);
    inS.readEmptyParams();
    bool ret = this->deactivateAgent(current);
    ::Ice::OutputStream* ostr = inS.startWriteParams();
    ostr->write(ret);
    inS.endWriteParams();
    return true;
}

bool
RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceD_getAgentState(::IceInternal::Incoming& inS, const ::Ice::Current& current)
{
    _iceCheckMode(::Ice::Normal, current.mode);
    inS.readEmptyParams();
    ::RoboCompAGMCommonBehavior::StateStruct ret = this->getAgentState(current);
    ::Ice::OutputStream* ostr = inS.startWriteParams();
    ostr->write(ret);
    inS.endWriteParams();
    return true;
}

bool
RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceD_getAgentParameters(::IceInternal::Incoming& inS, const ::Ice::Current& current)
{
    _iceCheckMode(::Ice::Normal, current.mode);
    inS.readEmptyParams();
    ::RoboCompAGMCommonBehavior::ParameterMap ret = this->getAgentParameters(current);
    ::Ice::OutputStream* ostr = inS.startWriteParams();
    ostr->write(ret);
    inS.endWriteParams();
    return true;
}

bool
RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceD_setAgentParameters(::IceInternal::Incoming& inS, const ::Ice::Current& current)
{
    _iceCheckMode(::Ice::Normal, current.mode);
    ::Ice::InputStream* istr = inS.startReadParams();
    ::RoboCompAGMCommonBehavior::ParameterMap iceP_prs;
    istr->read(iceP_prs);
    inS.endReadParams();
    bool ret = this->setAgentParameters(iceP_prs, current);
    ::Ice::OutputStream* ostr = inS.startWriteParams();
    ostr->write(ret);
    inS.endWriteParams();
    return true;
}

bool
RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceD_killAgent(::IceInternal::Incoming& inS, const ::Ice::Current& current)
{
    _iceCheckMode(::Ice::Normal, current.mode);
    inS.readEmptyParams();
    this->killAgent(current);
    inS.writeEmptyParams();
    return true;
}

bool
RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceD_uptimeAgent(::IceInternal::Incoming& inS, const ::Ice::Current& current)
{
    _iceCheckMode(::Ice::Normal, current.mode);
    inS.readEmptyParams();
    ::Ice::Int ret = this->uptimeAgent(current);
    ::Ice::OutputStream* ostr = inS.startWriteParams();
    ostr->write(ret);
    inS.endWriteParams();
    return true;
}

bool
RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceD_reloadConfigAgent(::IceInternal::Incoming& inS, const ::Ice::Current& current)
{
    _iceCheckMode(::Ice::Normal, current.mode);
    inS.readEmptyParams();
    bool ret = this->reloadConfigAgent(current);
    ::Ice::OutputStream* ostr = inS.startWriteParams();
    ostr->write(ret);
    inS.endWriteParams();
    return true;
}

namespace
{
const ::std::string iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_all[] =
{
    "activateAgent",
    "deactivateAgent",
    "getAgentParameters",
    "getAgentState",
    "ice_id",
    "ice_ids",
    "ice_isA",
    "ice_ping",
    "killAgent",
    "reloadConfigAgent",
    "setAgentParameters",
    "uptimeAgent"
};

}

bool
RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceDispatch(::IceInternal::Incoming& in, const ::Ice::Current& current)
{
    ::std::pair<const ::std::string*, const ::std::string*> r = ::std::equal_range(iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_all, iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_all + 12, current.operation);
    if(r.first == r.second)
    {
        throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
    }

    switch(r.first - iceC_RoboCompAGMCommonBehavior_AGMCommonBehavior_all)
    {
        case 0:
        {
            return _iceD_activateAgent(in, current);
        }
        case 1:
        {
            return _iceD_deactivateAgent(in, current);
        }
        case 2:
        {
            return _iceD_getAgentParameters(in, current);
        }
        case 3:
        {
            return _iceD_getAgentState(in, current);
        }
        case 4:
        {
            return _iceD_ice_id(in, current);
        }
        case 5:
        {
            return _iceD_ice_ids(in, current);
        }
        case 6:
        {
            return _iceD_ice_isA(in, current);
        }
        case 7:
        {
            return _iceD_ice_ping(in, current);
        }
        case 8:
        {
            return _iceD_killAgent(in, current);
        }
        case 9:
        {
            return _iceD_reloadConfigAgent(in, current);
        }
        case 10:
        {
            return _iceD_setAgentParameters(in, current);
        }
        case 11:
        {
            return _iceD_uptimeAgent(in, current);
        }
        default:
        {
            assert(false);
            throw ::Ice::OperationNotExistException(__FILE__, __LINE__, current.id, current.facet, current.operation);
        }
    }
}

void
RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceWriteImpl(::Ice::OutputStream* ostr) const
{
    ostr->startSlice(ice_staticId(), -1, true);
    Ice::StreamWriter< ::RoboCompAGMCommonBehavior::AGMCommonBehavior, ::Ice::OutputStream>::write(ostr, *this);
    ostr->endSlice();
}

void
RoboCompAGMCommonBehavior::AGMCommonBehavior::_iceReadImpl(::Ice::InputStream* istr)
{
    istr->startSlice();
    Ice::StreamReader< ::RoboCompAGMCommonBehavior::AGMCommonBehavior, ::Ice::InputStream>::read(istr, *this);
    istr->endSlice();
}

void
RoboCompAGMCommonBehavior::_icePatchObjectPtr(AGMCommonBehaviorPtr& handle, const ::Ice::ObjectPtr& v)
{
    handle = ::RoboCompAGMCommonBehavior::AGMCommonBehaviorPtr::dynamicCast(v);
    if(v && !handle)
    {
        IceInternal::Ex::throwUOE(::RoboCompAGMCommonBehavior::AGMCommonBehavior::ice_staticId(), v);
    }
}

namespace Ice
{
}

#endif
