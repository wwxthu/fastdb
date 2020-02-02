
using namespace db_common;

//-----------------------------------------------------------------------------
// common functions
//-----------------------------------------------------------------------------

template<class T>
void createRef(string name, dbReference<T>& ref)
{
  T rec;
  rec.Init(name);
  ref= insert(rec);
  //rec.m_ref= ref;
}

template<class T>
void findRef(string s, dbReference<T>& ref)
{
  try {
    dbQuery q;
    q="m_name=", s.c_str();
    dbCursor<T> cur(dbCursorViewOnly);
    int n= cur.select(q);
    if (n!=1)
    {
      MOTOR_TRACE("%d items of %s named %s found\n", n, T::ClassName().c_str(), s.c_str());
    }
    ref= cur.currentId();
  }
  catch (dbException e)
  {
    MOTOR_TRACE(e.getMsg());
    ref= null;
  }
}


template<class T>
T* FromRef(dbReference<T> ref)
{
  if (ref.isNull()) return NULL;
  dbCursor<T> cursor(dbCursorViewOnly);
  T* object= cursor.at(ref);
  return object;
}


//-----------------------------------------------------------------------------
// Transition functions
//-----------------------------------------------------------------------------

template <class Implementor, class State, class Automaton>
void TransitionT<Implementor, State, Automaton>::Init1(
  dbReference<EventT<Implementor> > event,
  dbReference<State> state,
  dbReference<State> nextState,
  dbReference<Automaton> subAutom,
  TransType tt,
  dbReference<ClosureT<Implementor> > fnCanHandle, 
  dbReference<ClosureT<Implementor> > fnHandle,
  dbReference<ClosureT<Implementor> > fnDiag)
{
  dbCursor<State> cur(dbCursorViewOnly);
  cur.select();
  State* st= cur.at(state);
  if (st) m_autom= st->m_autom;
  m_event= event;
  m_state= state;
  m_nextState= nextState;
  m_subAutomaton= subAutom;
  m_tt= tt;
  m_canHandleFunc= fnCanHandle;
  m_handleFunc= fnHandle;
  m_diagnosticFunc= fnDiag;
}


template <class Implementor, class State, class Automaton>
bool TransitionT<Implementor, State, Automaton>::Trigger()
{
  bool done= false;
  CONSTOBJ_BYREF(Closure, pCanHandle, m_canHandleFunc);
  if (!pCanHandle || (pCanHandle && (*pCanHandle)()))
  {
    CONSTOBJ_BYREF(Closure, pHandle, m_handleFunc);
    if (pHandle) (*pHandle)();
    CONSTOBJ_BYREF(State, pState, m_state);
    if (pState) pState->SetNext(m_nextState);
    CONSTOBJ_BYREF(Closure, pDiag, m_diagnosticFunc);
    if (pDiag) (*pDiag)();
    done= true;
  }
  return done;
}

//-----------------------------------------------------------------------------
// State functions
//-----------------------------------------------------------------------------

template<class Implementor, class Automaton>
void StateT<Implementor, Automaton>::Trigger(dbReference<Event> event)
{
  dbCursor<Transition> cur(dbCursorViewOnly);
  Transition *t= NULL;

  for (int i= 0; i< m_events.length(); ++i)
  {
    bool done= false;
    if (m_events[i]== event) 
    {
      if (!m_transitions[i].isNull())
      {
        t= cur.at(m_transitions[i]);
        if (t) done= t->Trigger();
        if (done) break;
      }
    }
  }
}


template<class Implementor, class Automaton>
void StateT<Implementor, Automaton>::SetNext(dbReference<StateT> nextState)
{
  DoHandler(m_onExit);
  {
    dbCursor<Automaton> curAutom(dbCursorForUpdate);
    Automaton* aut=curAutom.at(m_autom);
    aut->SetCurrentState(nextState); 
    curAutom.update();
  }
  dbDescriptor.getDatabase()->commit();
  dbDescriptor.getDatabase()->commit();
  CONSTOBJ_BYREF(StateT, pNext, nextState);
  if (pNext) pNext->DoHandler(pNext->m_onInit);
}


//-----------------------------------------------------------------------------
// Automaton functions
//-----------------------------------------------------------------------------

template<class Implementor, class StateMachine>
void AutomatonT<Implementor, StateMachine>::Trigger(dbReference<Event> event)
{
  CONSTOBJ_BYREF(State, pState, m_currentState);
  if (pState) pState->Trigger(event);
}


//-----------------------------------------------------------------------------
// Statemachine functions
//-----------------------------------------------------------------------------

template<class Implementor>
void StateMachineT<Implementor>::Trigger(dbReference<Event> event)
{
  CONSTOBJ_BYREF(Automaton, pAutom, m_currentAutomaton);
  if (pAutom) pAutom->Trigger(event);
  dbDescriptor.getDatabase()->commit();
}
