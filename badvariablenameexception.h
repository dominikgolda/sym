#ifndef BADVARIABLENAMEEXCEPTION_H
#define BADVARIABLENAMEEXCEPTION_H
// klasa skopiowana stąd http://stackoverflow.com/questions/8152720/correct-way-to-inherit-from-stdexception
#include<QString>
class BadVariableNameException : public std::exception
{
public:
    /** Constructor (C strings).
     *  @param message C-style string error message.
     *                 The string contents are copied upon construction.
     *                 Hence, responsibility for deleting the \c char* lies
     *                 with the caller.
     */
    explicit BadVariableNameException(const char* message):
      msg_(message)
      {
      }

    /** Constructor (QStrings strings).
     *  @param message The error message.
     */
    explicit BadVariableNameException(const QString& message);


    /** Destructor.
     * Virtual to allow for subclassing.
     */
    virtual ~BadVariableNameException() throw (){}

    /** Returns a pointer to the (constant) error description.
     *  @return A pointer to a \c const \c char*. The underlying memory
     *          is in posession of the \c BadVariableNameException object. Callers \a must
     *          not attempt to free the memory.
     */
    virtual const char* what() const throw (){
       return msg_.toLocal8Bit().data();
    }

protected:
    /** Error message.
     */
    QString msg_;
};
#endif // BADVARIABLENAMEEXCEPTION_H
