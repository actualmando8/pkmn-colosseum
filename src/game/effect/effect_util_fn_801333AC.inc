    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x2
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    li r3, 0x0
    bl fn_800F9318
    cmplwi r3, 0x0
    bne @L_801333DC
    li r3, 0x0
    b @L_8013343C
    @L_801333DC:
    cmpwi r31, 0xb9
    beq @L_80133418
    bge @L_80133438
    cmpwi r31, 0xb8
    bge @L_801333F4
    b @L_80133438
    @L_801333F4:
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8013340C
    li r0, 0x0
    stb r0, 0x0(r3)
    b @L_80133438
    @L_8013340C:
    li r0, 0x1
    stb r0, 0x0(r3)
    b @L_80133438
    @L_80133418:
    lbz r0, 0x1(r3)
    cmplwi r0, 0x0
    beq @L_80133430
    li r0, 0x0
    stb r0, 0x1(r3)
    b @L_80133438
    @L_80133430:
    li r0, 0x1
    stb r0, 0x1(r3)
    @L_80133438:
    li r3, 0x0
    @L_8013343C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
