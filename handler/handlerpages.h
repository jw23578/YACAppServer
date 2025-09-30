#ifndef HANDLERPAGES_H
#define HANDLERPAGES_H

#include "interfaces/pistachehandlerinterface.h"

class HandlerPages : public PistacheHandlerInterface
{
public:
    HandlerPages(PistacheServerInterface &serverInterface,
                 ORMPersistenceInterface &opi);

    // PistacheHandlerInterface interface
public:
    void method(CurrentContext &context) override;
};

#endif // HANDLERPAGES_H
