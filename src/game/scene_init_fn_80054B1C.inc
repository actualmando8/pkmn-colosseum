    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    mr r30, r4
    stw r30, 0x8(r1)
    stw r29, 0xc(r1)
    bl fn_801046B8
    mr r4, r3
    addi r9, r1, 0x8
    li r3, 0x9c
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    mr r31, r3
    li r3, 0x9c
    bl fn_80102510
    li r3, 0x9c
    li r4, 0x1
    bl fn_80102428
    cmpwi r31, 0x2
    beq @L_80054BFC
    bge @L_80054BA0
    cmpwi r31, 0x0
    beq @L_80054BB0
    bge @L_80054BF4
    b @L_80054C24
    @L_80054BA0:
    cmpwi r31, 0x4
    beq @L_80054C1C
    bge @L_80054C24
    b @L_80054C14
    @L_80054BB0:
    cmpwi r29, 0x1
    beq @L_80054BDC
    bge @L_80054BC8
    cmpwi r29, 0x0
    bge @L_80054BD4
    b @L_80054BEC
    @L_80054BC8:
    cmpwi r29, 0x3
    bge @L_80054BEC
    b @L_80054BE4
    @L_80054BD4:
    li r3, 0x0
    b @L_80054C28
    @L_80054BDC:
    li r3, 0x1
    b @L_80054C28
    @L_80054BE4:
    li r3, 0x2
    b @L_80054C28
    @L_80054BEC:
    li r3, 0x0
    b @L_80054C28
    @L_80054BF4:
    li r3, 0x3
    b @L_80054C28
    @L_80054BFC:
    cmpwi r30, 0x0
    beq @L_80054C0C
    li r3, 0x5
    b @L_80054C28
    @L_80054C0C:
    li r3, 0x4
    b @L_80054C28
    @L_80054C14:
    li r3, 0x6
    b @L_80054C28
    @L_80054C1C:
    li r3, 0x7
    b @L_80054C28
    @L_80054C24:
    li r3, 0x8
    @L_80054C28:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
