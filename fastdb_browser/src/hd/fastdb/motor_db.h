#ifndef __MOTOR_DB_H__
#define __MOTOR_DB_H__

#pragma warning(disable:4786)

#include "fastdb/fastdb.h"

#define MOTOR_TRACE ATLTRACE

using namespace std;

#include "motor_mac.inl"

namespace db_common
{

  enum TransType
  {
    undef, empty, event, push, pop
  };


  //-----------------------------------------------------------------------------
  // EngineOject: common root object for all statemachine classes
  //-----------------------------------------------------------------------------

  template <class Implementor>

  struct EngineObjectT
  {
    EngineObjectT()
    {}

    EngineObjectT(string name)
    {
      Init(name);
    }

    void Init(string name)
    {
      m_name= name;
    }

    string GetName()
    {
      return m_name;
    }

    string m_name;

    static string ClassName()
    {
      return "EngineObject";
    }

    TYPE_DESCRIPTOR((KEY(m_name, HASHED)
                    ));
  };


  //-----------------------------------------------------------------------------
  // Event: representing events which trigger statemachine
  //-----------------------------------------------------------------------------

  template <class Implementor>

  struct EventT : public EngineObjectT<Implementor>
  {
    static string ClassName()
    {
      return "Event";
    }

    TYPE_DESCRIPTOR((SUPERCLASS(EngineObjectT<Implementor>)));
  };


  //-----------------------------------------------------------------------------
  // Closure: encapsulates eventhandler instance and method
  //-----------------------------------------------------------------------------

  template <class Implementor>

  class ClosureT : public EngineObjectT<Implementor>
  {

  public:
    typedef bool (Implementor::*Action)(dbAnyReference automaton);

    ClosureT()
    {
      m_obj= NULL;
    }

    ClosureT(string name, Implementor* obj, Action method, dbAnyReference automaton)
        : EngineObjectT<Implementor>(name), m_obj(obj), m_method(method), m_automaton(automaton)
    {}

    bool operator()()
    {
      return (m_obj->*m_method)(m_automaton);
    }

  private:
    Implementor* m_obj;
    Action m_method;
    dbAnyReference m_automaton;

  public:
    static string ClassName()
    {
      return "Closure";
    }

    TYPE_DESCRIPTOR((SUPERCLASS(EngineObjectT<Implementor>),
                     RAWFIELD(m_obj),
                     RAWFIELD(m_method),
                     FIELD(m_automaton)
                    ));

  };


  //-----------------------------------------------------------------------------
  // Transition: definition of statechange/actions that occur on an event specific to a state
  //-----------------------------------------------------------------------------

  template <class Implementor, class State, class Automaton>

  class TransitionT : public EngineObjectT<Implementor>
  {

  public:
    typedef ClosureT<Implementor> Closure;

    void Init1(dbReference<EventT<Implementor> > event,
               dbReference<State> state,
               dbReference<State> nextState,
               dbReference<Automaton> subAutom,
               TransType tt,
               dbReference<ClosureT<Implementor> > fnCanHandle,
               dbReference<ClosureT<Implementor> > fnHandle,
               dbReference<ClosureT<Implementor> > fnDiag);

    bool Trigger();

    dbReference<EventT<Implementor> > m_event;
    dbReference<Automaton> m_autom;
    dbReference<Automaton> m_subAutomaton;
    dbReference<State> m_state;
    dbReference<State> m_nextState;
    dbReference<ClosureT<Implementor> > m_canHandleFunc;
    dbReference<ClosureT<Implementor> > m_handleFunc;
    dbReference<ClosureT<Implementor> > m_diagnosticFunc;
    int4 m_tt;

    static string ClassName()
    {
      return "Transition";
    }

    TYPE_DESCRIPTOR((SUPERCLASS(EngineObjectT<Implementor>),
                     FIELD(m_event),
                     FIELD(m_autom),
                     FIELD(m_subAutomaton),
                     RELATION(m_state, m_transitions),
                     FIELD(m_nextState),
                     FIELD(m_canHandleFunc),
                     FIELD(m_handleFunc),
                     FIELD(m_diagnosticFunc),
                     FIELD(m_tt)
                    ));
  };


  //-----------------------------------------------------------------------------
  // State: state of a specific automaton/subautomaton of the statemachine
  //-----------------------------------------------------------------------------

  template<class Implementor, class Automaton>

  class StateT : public EngineObjectT<Implementor>
  {

  public:
    typedef TransitionT<Implementor, StateT<Implementor,Automaton>, Automaton> Transition;
    typedef EventT<Implementor> Event;
    typedef ClosureT<Implementor> Closure;

    void Add(dbReference<Transition> trans)
    {
      //m_transitions.append(trans);
      CONSTOBJ_BYREF(Transition, pTrans, trans);

      if (pTrans)
      {
        m_events.append(pTrans->m_event);
      }
    }

    void Trigger(dbReference<Event> event);

    void SetNext(dbReference<StateT> nextState);

    void DoHandler(dbReference<Closure> handler)
    {
      CONSTOBJ_BYREF(Closure, pHandler, handler);

      if (pHandler)
        (*pHandler)();
    }

    dbReference<Automaton> m_autom;
    dbReference<Closure> m_onInit;
    dbReference<Closure> m_onExit;
    dbArray< dbReference<Transition> > m_transitions;
    dbArray< dbReference<Event> > m_events;
    dbReference<Transition> m_currentTransition;
    dbReference<EventT<Implementor> > m_currentEvent;

  public:
    static string ClassName()
    {
      return "State";
    }

    TYPE_DESCRIPTOR((SUPERCLASS(EngineObjectT<Implementor>),
                     RELATION(m_autom, m_states),
                     FIELD(m_onInit),
                     FIELD(m_onExit),
                     OWNER(m_transitions, m_state),
                     FIELD(m_events),
                     FIELD(m_currentTransition),
                     FIELD(m_currentEvent)
                    ));
  };


  //-----------------------------------------------------------------------------
  // Automaton: main automaton or subautomaton of a statemachine
  //-----------------------------------------------------------------------------

  template<class Implementor, class StateMachine>

  class AutomatonT : public EngineObjectT<Implementor>
  {

  public:
    typedef StateT<Implementor, AutomatonT<Implementor, StateMachine> > State;
    typedef EventT<Implementor> Event;

    void Add(dbReference<State> state)
    {
      m_states.append(state);
    }

    void SetStartState(dbReference<State> state)
    {
      m_currentState= m_firstState= state;
    }

    void SetCurrentState(dbReference<State>state)
    {
      m_currentState= state;
    }

    void Trigger(dbReference<Event> event);

    dbArray< dbReference<State> > m_states;
    dbReference<State> m_firstState;
    dbReference<State> m_currentState;
    dbReference<StateMachine> m_motor;

  public:
    static string ClassName()
    {
      return "Automaton";
    }

    TYPE_DESCRIPTOR((SUPERCLASS(EngineObjectT<Implementor>),
                     OWNER(m_states, m_autom),
                     FIELD(m_firstState),
                     FIELD(m_currentState),
                     RELATION(m_motor, m_automatons)
                    ));
  };

  //-----------------------------------------------------------------------------
  // Statemachine: controlling class for statemachine
  //-----------------------------------------------------------------------------

  template<class Implementor>

  class StateMachineT : public EngineObjectT<Implementor>
  {

  public:
    typedef AutomatonT<Implementor, StateMachineT<Implementor> > Automaton;
    typedef StateT<Implementor, AutomatonT<Implementor, StateMachineT> > State;
    typedef EventT<Implementor> Event;

    void Add(dbReference<Automaton> autom)
    {
      m_automatons.append(autom);
    }

    void SetStartAutomaton(dbReference<Automaton> automaton)
    {
      m_currentAutomaton= automaton;
      m_firstAutomaton= automaton;
    }

    void SetCurrentAutomaton(dbReference<Automaton> automaton)
    {
      m_currentAutomaton= automaton;
    }

    void Trigger(dbReference<Event> event);

    void Trigger(string event)
    {
      dbReference<Event> evtRef;
      findRef(event, evtRef);

      if (evtRef != null)
        Trigger(evtRef);
    }

    dbArray< dbReference<Automaton> > m_automatons;
    dbReference<Automaton> m_firstAutomaton;
    dbReference<Automaton> m_currentAutomaton;

  public:
    static string ClassName()
    {
      return "StateMachine";
    }

    TYPE_DESCRIPTOR((SUPERCLASS(EngineObjectT<Implementor>),
                     OWNER(m_automatons, m_motor),
                     FIELD(m_firstAutomaton),
                     FIELD(m_currentAutomaton)
                    ));
  };

};

#include "motor_db.inl"
#endif
