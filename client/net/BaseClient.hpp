#include <string>

#include <Python.h>


namespace net {


class BaseClient {
    public:
        BaseClient();
        virtual ~BaseClient();

        /// Process all available messages.
        ///
        /// This will pull all available messages from the Python client and
        /// pass them through processMessage.
        ///
        /// This calls the Python client's `retrieve` method and expects it
        /// to return an iterator of messages. If an iterator is not returned
        /// then a std::runtime_error is thrown.
        ///
        /// Each message is also expected to have `entity` and `type`
        /// attributes. The entity can be any Python object, and will be pass
        /// as-is to processMessage. The type is expected to have its own
        /// `name` attribute. This name is converted to a std::string before
        /// being passed to processMessage.
        ///
        /// If any message in the iterator doesn't conform to these
        /// expectations then it is ignored and processMessage will never be
        /// called for it.
        ///
        /// @returns the number of messages that were processed which
        ///     may be zero.
        unsigned int process();

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
        void send(std::string type, PyObject* entity);

    private:
        long m_tid;
        PyThreadState * m_tstate;
        unsigned short m_tstate_count;
        PyObject * m_py_client;
        PyObject * m_py_client_send;
        PyObject * m_py_client_retrieve;

        /// Call the Python entry point function.
        ///
        /// This calls a Python function which is expected to return a client
        /// thread which has a 'send' and 'retrieve' method. The m_py_client,
        /// m_py_client_send and m_py_client_retrieve members will be set to
        /// the respctive Python objects.
        ///
        /// If the object returned by calling the entry point meets the
        /// expectations then true is retured. However, if any one of the
        /// expectations is not met then false is returned and ``error`` is
        /// set to an error message.
        bool invokeEntryPoint(PyObject * ep, const char ** error);

        virtual void processMessage(std::string type, PyObject* entity) = 0;
};


}  // namespace net
