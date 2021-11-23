#if defined(Host)
    #define INVALID -1
    #define ADD 0
    #define ALSO 1
    #define AND 2
    #define ASSIGN 3
    #define BLANK 4
    #define COBEGIN 5
    #define CONSTANT 6
    #define CONSTRUCT 7
    //  #define DIFFERENCE 8
    #define DIVIDE 9
    #define DO 10
    #define ELSE 11
    #define ENDCODE 12
    #define ENDIF 13
    #define ENDLIB 14
    #define ENDPROC 15
    #define ENDWHEN 16
    #define EQUAL 17
    #define ERROR 18
    #define FIELD 19
    //  #define FUNCVAL 20
    #define GOTO 21
    #define GREATER 22
    //  #define IN 23
    #define INDEX 24
    #define INSTANCE 25
    //  #define INTERSECTION 26
    #define LESS 27
    #define LIBPROC 28
    #define MINUS 29
    #define MODULO 30
    #define MULTIPLY 31
    #define NEWLINE 32
    #define NOT 33
    #define NOTEQUAL 34
    #define NOTGREATER 35
    #define NOTLESS 36
    #define OR 37
    #define PARAMARG 38
    #define PARAMCALL 39
    #define PARAMETER 40
    #define PROCARG 41
    #define PROCCALL 42
    #define PROCEDURE 43
    #define PROCESS 44
    #define SUBTRACT 45
    //  #define UNION 46
    #define VALSPACE 47
    #define VALUE 48
    #define VARIABLE 49
    #define WAIT 50
    #define WHEN 51
    #define WHILE 52
    #define ADDR 53
    #define HALT 54
    #define OBTAIN 55
    #define PLACE 56
    #define SENSE 57
    /** Extra */
    #define ELEMASSIGN 58
    #define ELEMVALUE 59
    #define LOCALCASE 60
    #define LOCALSET 61
    #define LOCALVALUE 62
    #define LOCALVAR 63
    #define OUTERCALL 64
    #define OUTERCASE 65
    #define OUTERPARAM 66
    #define OUTERSET 67
    #define OUTERVALUE 68
    #define OUTERVAR 69
    #define SETCONST 70
    #define SINGLETON 71
    #define STRINGCONST 72

    #define PUTI 73
    #define PUTC 74
    #define PUTB 75
    #define PUTN 76

    #define MAX_KERNEL_STACK_SIZE 100
    #define MAX_QUEUE 10
    #define MIN_ADDRESS 0
    #define MAX_ADDRESS 20000 // 1024
    #define SPACE (int)(' ')
    #define INSTR_TABLE 400
    #define SET_LENGTH 0x8
    #define SET_LIMIT 127
#else // on avr
    #define INVALID 0xFFFF
    #define ADD 0x0000
    #define ALSO 0x0001
    #define AND 0x0002
    #define ASSIGN 0x0003
    #define BLANK 0x0004
    #define COBEGIN 0x0005
    #define CONSTANT 0x0006
    #define CONSTRUCT 0x0007
    //  #define DIFFERENCE 8
    #define DIVIDE 0x0009
    #define DO 0x000A
    #define ELSE 0x000B
    #define ENDCODE 12
    #define ENDIF 0x000C
    #define ENDLIB 0x000E
    #define ENDPROC 0x000F
    #define ENDWHEN 0x0010
    #define EQUAL 0x0011
    #define ERROR 0x0012
    #define FIELD 0x0013
    //  #define FUNCVAL 20
    #define GOTO 0x0015
    #define GREATER 0x0016
    //  #define IN 23
    #define INDEX 0x0018
    #define INSTANCE 0x0019
    //  #define INTERSECTION 26
    #define LESS 0x001B
    #define LIBPROC 0x001C
    #define MINUS 0x001D
    #define MODULO 0x001E
    #define MULTIPLY 0x001F
    #define NEWLINE 0x0020
    #define NOT 0x0021
    #define NOTEQUAL 0x0022
    #define NOTGREATER 0x0023
    #define NOTLESS 0x0024
    #define OR 0x0025
    #define PARAMARG 0x0026
    #define PARAMCALL 0x0027
    #define PARAMETER 0x0028
    #define PROCARG 0x0029
    #define PROCCALL 0x002A
    #define PROCEDURE 0x002B
    #define PROCESS 0x002C
    #define SUBTRACT 0x002D
    //  #define UNION 46
    #define VALSPACE 0x002F
    #define VALUE 0x0030
    #define VARIABLE 0x0031
    #define WAIT 0x0032
    #define WHEN 0x0033
    #define WHILE 0x0034
    #define ADDR 0x0035
    #define HALT 0x0036
    #define OBTAIN 0x0037
    #define PLACE 0x0038
    #define SENSE 0x0039
    /** Extra */
    #define ELEMASSIGN 0x003A
    #define ELEMVALUE 0x003B
    #define LOCALCASE 0x003C
    #define LOCALSET 0x003D
    #define LOCALVALUE 0x003E
    #define LOCALVAR 0x003F
    #define OUTERCALL 0x0040
    #define OUTERCASE 0x0041
    #define OUTERPARAM 0x0042
    #define OUTERSET 0x0043
    #define OUTERVALUE 0x0044
    #define OUTERVAR 0x0045
    #define SETCONST 0x0046
    #define SINGLETON 0x0047
    #define STRINGCONST 0x0048

    #define PUTI 0x0049
    #define PUTC 0x004A
    #define PUTB 0x004B
    #define PUTN 0x004C

    #define MAX_KERNEL_STACK_SIZE 0x0064
    #define MAX_QUEUE 0x000A
    #define MIN_ADDRESS 0x0000
    #define MAX_ADDRESS 0x4E20 // 0x0400
    #define SPACE 0x0020
    #define INSTR_TABLE 0x0190
    #define SET_LENGTH 0x8
    #define SET_LIMIT 0x007F
#endif