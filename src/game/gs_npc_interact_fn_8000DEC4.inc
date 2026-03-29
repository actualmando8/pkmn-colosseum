    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    li r4, 0x0
    bl fn_801040D0
    li r5, 0x0
    li r0, 0x2
    mtctr r0
    @L_8000DEEC:
    lwz r0, 0x4(r3)
    cmpwi r0, 0x47
    beq @L_8000DF28
    bge @L_8000DF0C
    cmpwi r0, 0x45
    beq @L_8000DF18
    bge @L_8000DF20
    b @L_8000DF38
    @L_8000DF0C:
    cmpwi r0, 0x49
    bge @L_8000DF38
    b @L_8000DF30
    @L_8000DF18:
    li r4, 0x125c
    b @L_8000DF3C
    @L_8000DF20:
    li r4, 0x125e
    b @L_8000DF3C
    @L_8000DF28:
    li r4, 0x12be
    b @L_8000DF3C
    @L_8000DF30:
    li r4, 0x125d
    b @L_8000DF3C
    @L_8000DF38:
    li r4, 0x0
    @L_8000DF3C:
    lha r0, 0x6(r31)
    cmpw r4, r0
    bne @L_8000DF58
    mr r3, r31
    li r4, 0x1
    bl fn_80109220
    b @L_8000DFDC
    @L_8000DF58:
    lwz r0, 0xc(r3)
    cmpwi r0, 0x47
    beq @L_8000DF94
    bge @L_8000DF78
    cmpwi r0, 0x45
    beq @L_8000DF84
    bge @L_8000DF8C
    b @L_8000DFA4
    @L_8000DF78:
    cmpwi r0, 0x49
    bge @L_8000DFA4
    b @L_8000DF9C
    @L_8000DF84:
    li r4, 0x125c
    b @L_8000DFA8
    @L_8000DF8C:
    li r4, 0x125e
    b @L_8000DFA8
    @L_8000DF94:
    li r4, 0x12be
    b @L_8000DFA8
    @L_8000DF9C:
    li r4, 0x125d
    b @L_8000DFA8
    @L_8000DFA4:
    li r4, 0x0
    @L_8000DFA8:
    lha r0, 0x6(r31)
    cmpw r4, r0
    bne @L_8000DFC4
    mr r3, r31
    li r4, 0x1
    bl fn_80109220
    b @L_8000DFDC
    @L_8000DFC4:
    addi r3, r3, 0x10
    addi r5, r5, 0x1
    bdnz @L_8000DEEC
    mr r3, r31
    li r4, 0x0
    bl fn_80109220
    @L_8000DFDC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
