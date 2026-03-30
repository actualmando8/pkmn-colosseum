    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    li r30, 0x0
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r4
    bne @L_80134820
    li r3, 0x0
    li r4, 0x3
    bl fn_80129280
    @L_80134820:
    extsb. r0, r28
    blt @L_80134834
    extsb r0, r28
    cmpwi r0, 0x3
    blt @L_8013483C
    @L_80134834:
    li r30, -0x1
    b @L_801348B4
    @L_8013483C:
    mulli r0, r0, 0x24a4
    li r31, 0x0
    add r29, r3, r0
    b @L_801348A8
    @L_8013484C:
    extsb. r0, r28
    blt @L_80134860
    extsb r0, r28
    cmpwi r0, 0x3
    blt @L_80134868
    @L_80134860:
    li r3, 0x0
    b @L_80134888
    @L_80134868:
    extsb. r0, r31
    blt @L_8013487C
    extsb r0, r31
    cmpwi r0, 0x1e
    blt @L_80134884
    @L_8013487C:
    li r3, 0x0
    b @L_80134888
    @L_80134884:
    addi r3, r29, 0x14
    @L_80134888:
    cmplwi r3, 0x0
    beq @L_801348A0
    bl fn_80123FBC
    clrlwi. r0, r3, 24
    beq @L_801348A0
    addi r30, r30, 0x1
    @L_801348A0:
    addi r29, r29, 0x138
    addi r31, r31, 0x1
    @L_801348A8:
    extsb r0, r31
    cmpwi r0, 0x1e
    blt @L_8013484C
    @L_801348B4:
    extsb. r0, r30
    bge @L_801348C4
    li r3, -0x1
    b @L_801348CC
    @L_801348C4:
    subfic r0, r30, 0x1e
    extsb r3, r0
    @L_801348CC:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
