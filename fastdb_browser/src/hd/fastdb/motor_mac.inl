//-----------------------------------------------------------------------------
// macros to create statemachines
//-----------------------------------------------------------------------------

#define DECLARE_STATEMACHINE(ImplementClass) \
typedef StateMachineT<ImplementClass> STATEMACHINE_PREFIX()##StateMachine;\
typedef AutomatonT<ImplementClass, STATEMACHINE_PREFIX()##StateMachine> STATEMACHINE_PREFIX()##Automaton;\
typedef StateT<ImplementClass, STATEMACHINE_PREFIX()##Automaton> STATEMACHINE_PREFIX()##State;\
typedef TransitionT<ImplementClass, STATEMACHINE_PREFIX()##State, STATEMACHINE_PREFIX()##Automaton> STATEMACHINE_PREFIX()##Transition;\
typedef ClosureT<ImplementClass> STATEMACHINE_PREFIX()##Closure;\
typedef EventT<ImplementClass> STATEMACHINE_PREFIX()##Event;\
typedef EngineObjectT<ImplementClass> STATEMACHINE_PREFIX()##EngineObject;

#define REGISTER_STATEMACHINE() \
REGISTER(STATEMACHINE_PREFIX()##EngineObject);\
REGISTER(STATEMACHINE_PREFIX()##Event);\
REGISTER(STATEMACHINE_PREFIX()##StateMachine);\
REGISTER(STATEMACHINE_PREFIX()##Automaton);\
REGISTER(STATEMACHINE_PREFIX()##State);\
REGISTER(STATEMACHINE_PREFIX()##Transition);\
REGISTER(STATEMACHINE_PREFIX()##Closure);

#define CREATE_REF(type, name, var) dbReference<STATEMACHINE_PREFIX()##type> var; createRef(#name, var);
#define SHORT_REF(type, name) dbReference<STATEMACHINE_PREFIX()##type> name; createRef(#name, name);
#define ADD_REF(type, var, ref) { dbCursor<STATEMACHINE_PREFIX()##type> cur(dbCursorForUpdate); \
  STATEMACHINE_PREFIX()##type* coll= cur.at(var); coll->Add(ref); cur.update(); }


#define ADD_TRANSITION(trans, event, state, nextState, subAutom, tt, fnCanHandle, fnHandle, fnDiag) \
  { SHORT_REF(Transition, trans);  \
    dbCursor<STATEMACHINE_PREFIX()##Transition> cur(dbCursorForUpdate);\
    STATEMACHINE_PREFIX()##Transition* t=cur.at(trans);\
    t->Init1(event, state, nextState, subAutom, tt, fnCanHandle, fnHandle, fnDiag);\
    cur.update();  ADD_REF(State, state, trans); } 

#define CONSTOBJ_BYREF(type, rec, ref) \
  dbCursor<type> cur##rec(dbCursorViewOnly); type* rec=NULL; \
  if (!ref.isNull()) rec= cur##rec.at(ref); 

#define OBJ_BYREF(type, rec, ref) \
  dbCursor<type> cur##rec(dbCursorForUpdate); type* rec=NULL; \
  if (!ref.isNull()) rec= cur##rec.at(ref);

#define CONSTOBJ_CASTED(type, rec, ref) \
  dbCursor<type> cur##rec(dbCursorViewOnly); type* rec=NULL; \
  dbReference<type> ref##rec; ref##rec.unsafeAssign(ref); \
  if (!ref.isNull()) rec= cur##rec.at(ref##rec);

#define OBJ_CASTED(type, rec, ref) \
  dbCursor<type> cur##rec(dbCursorForUpdate); type* rec=NULL; \
  dbReference<type> ref##rec; ref##rec.unsafeAssign(ref); \
  if (!ref.isNull()) rec= cur##rec.at(ref##rec);

#define SET_START_AUTOMATON(engine, automref) {\
    dbCursor<STATEMACHINE_PREFIX()##StateMachine> curEng(dbCursorForUpdate);\
    curEng.at(engine)->SetStartAutomaton(automref); curEng.update(); }

#define SET_START_STATE(automref, stateref) {\
    dbCursor<STATEMACHINE_PREFIX()##Automaton> curAutom(dbCursorForUpdate);\
    curAutom.at(automref)->SetStartState(stateref); curAutom.update(); }

#define SET_CURRENT_AUTOMATON(engine, automref) {\
    dbCursor<STATEMACHINE_PREFIX()##StateMachine> curEng(dbCursorForUpdate);\
    curEng.at(engine)->SetCurrentAutomaton(automref); curEng.update(); }

#define SET_CURRENT_STATE(automref, stateref) {\
    dbCursor<STATEMACHINE_PREFIX()##Automaton> curAutom(dbCursorForUpdate);\
    curAutom.at(automref)->SetCurrentState(stateref); curAutom.update(); }


#define CLOSURE(controller, obj, method, automaton) \
  STATEMACHINE_PREFIX()##Closure clos##method(#method, obj, &controller::method, automaton);\
  dbReference<STATEMACHINE_PREFIX()##Closure> method= insert(clos##method);

#define WIPE_TABLE(table) { dbCursor<STATEMACHINE_PREFIX()##table> cur(dbCursorForUpdate);\
  cur.removeAll(); }

#define INIT_STATEMACHINE() \
  WIPE_TABLE(Closure); WIPE_TABLE(Event); WIPE_TABLE(Transition); \
  WIPE_TABLE(State); WIPE_TABLE(Automaton); WIPE_TABLE(StateMachine); 

#define DO_COMMIT() dbDescriptor.getDatabase()->commit();