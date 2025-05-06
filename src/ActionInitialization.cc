#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

ActionInitialization::ActionInitialization() {}
ActionInitialization::~ActionInitialization() {}

void ActionInitialization::Build() const {
    SetUserAction(new PrimaryGeneratorAction());   // Fuente Cf-252 vía wrapper
    auto runAction = new RunAction();
    SetUserAction(runAction);

    auto eventAction = new EventAction();
    SetUserAction(eventAction);

    SetUserAction(new SteppingAction(eventAction));
}
