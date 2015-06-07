#include <string>

#include <Python.h>


namespace net {


class BaseClient {
    public:
        BaseClient();
        virtual ~BaseClient();

        void pump();

    protected:
        /// Re-enterant version of `PyEval_SaveThread`.
        ///
        /// This should be called whenever you want to release control back
        /// to the Python-managed thread. It should only be called after a
        /// previous call is made to `restoreThread`.
        void saveThread();

        /// Re-enterant version of `PyEval_RestoreThread`.
        ///
        /// This will claim the Python GIL and hence should be called before
        /// interacting with any Python objects. Once finished with the Python
        /// objects `saveThread` should be called in order to release the GIL.
        ///
        /// As this locks the interpreter, any Python threads will be blocked
        /// until `saveThread` is called.
        ///
        /// It is safe to call this multiple times as long as an equal number
        /// of `saveThread` calls is made.
        void restoreThread();

        /// Push a message onto the Python client's queue.
        ///
        /// This steals the reference to the `entity`.
        void pushMessage(std::string type, PyObject* entity);

    private:
        PyThreadState* m_tstate;
        unsigned short m_tstate_count;
        PyObject* m_py_client;
        PyObject* m_py_messages;
        PyObject* m_py_o_messages;

        virtual void convert(std::string type, PyObject* entity) = 0;
};


}  // namespace net
