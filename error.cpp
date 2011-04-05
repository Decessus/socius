#include "error.h"

ErrorManager::ErrorManager(QObject *parent) : QThread(parent) {

}

ErrorManager::run() {
    forever {
        while(!Errors.isEmpty()) {
            ErrorData TempError = Errors.dequeue();

            switch ()
        }
    }
}
