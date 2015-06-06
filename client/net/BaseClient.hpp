#include <string>

#include <Python.h>


namespace net {


class BaseClient {
    public:
        BaseClient();
        ~BaseClient();

        void pump();

    protected:
        /// Push a message onto the Python client's queue.
        ///
        /// This steals the reference to the `entity`.
        void pushMessage(std::string type, PyObject* entity);
        PyThreadState* m_py_tstate;

    private:
        PyObject* m_py_client;
        PyObject* m_py_messages;
        PyObject* m_py_o_messages;

        virtual void convert(std::string type, PyObject* entity) = 0;
};


}  // namespace net
