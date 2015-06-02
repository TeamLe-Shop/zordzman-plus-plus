#include <string>

#include <Python.h>


namespace net {


class BaseClient {
    public:
        BaseClient();
        ~BaseClient();

        void pump();

    private:
        PyThreadState* m_py_tstate;
        PyObject* m_py_client;
        PyObject* m_py_messages;

        virtual void convert(std::string type, PyObject* entity) = 0;
};


}  // namespace net
