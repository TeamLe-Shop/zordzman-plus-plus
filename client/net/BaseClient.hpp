#include <string>

#include <Python.h>


namespace net {


class BaseClient {
    public:
        BaseClient();
        ~BaseClient();

        void pump();

    protected:
        PyThreadState* m_py_tstate;

    private:
        PyObject* m_py_client;
        PyObject* m_py_messages;

        virtual void convert(std::string type, PyObject* entity) = 0;
};


}  // namespace net
