    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r3, 0x8046
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    li r29, 0x0
    li r31, 0x0
    lwzu r30, 31936(r3)
    lhz r28, 0x4(r3)
    b @L_801DA820
@L_801DA7D4:
    lbz r0, 0x74(r30)
    cmplwi r0, 0x0
    beq @L_801DA818
    lwz r27, 0x68(r30)
    b @L_801DA80C
@L_801DA7E8:
    lbz r0, 0x14(r27)
    lwz r26, 0x34(r27)
    cmplwi r0, 0x0
    beq @L_801DA800
    mr r3, r27
    bl fn_801DBB10
@L_801DA800:
    mr r3, r27
    bl fn_801DBDDC
    mr r27, r26
@L_801DA80C:
    cmplwi r27, 0x0
    bne @L_801DA7E8
    stw r31, 0x68(r30)
@L_801DA818:
    addi r29, r29, 0x1
    addi r30, r30, 0x8c
@L_801DA820:
    cmpw r29, r28
    blt @L_801DA7D4
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
