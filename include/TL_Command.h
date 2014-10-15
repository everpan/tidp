/* 
 * File:   TL_Command.h
 * Author: everpan
 *
 * Created on 2011年8月11日, 下午5:59
 */

#ifndef TL_COMMAND_H
#define	TL_COMMAND_H
namespace tidp {

    class TL_Command {
    public:
        TL_Command();
        virtual ~TL_Command();
        virtual void Execute() = 0;
        virtual void UnExecute() = 0;
    private:

    };


}
#endif	/* TL_COMMAND_H */

