 /* File : hello.x
    
         This is the protocol definition file. The programmer writes
         this file using the RPC language. This file is passed to the
         protocol generator rpcgen. Every remote procedure is part of
         a remote program. Each procedure has a name and number. A
         version number is also supplied so different versions of the
         same procedure may be generated.
     */
     program DISPLAY_PRG {
       version DISPLAY_VER {
         int print_hello( void ) = 1;
       } = 1;
     } = 0x20000001;