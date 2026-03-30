    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r31, r3
    lbz r0, 0x2(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8006DC00
    lbz r0, 0x1(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    beq @L_8006DB1C
    b @L_8006DC00
    @L_8006DB1C:
    li r4, 0x0
    bl fn_801040D0
    mr r29, r3
    mr r3, r31
    li r4, 0x1
    bl fn_801040D0
    mr r30, r3
    mr r3, r31
    li r4, 0x2
    bl fn_801040D0
    mr r0, r3
    mr r3, r31
    mr r28, r0
    li r4, 0x3
    bl fn_801040D0
    extsh r5, r3
    li r3, 0xd6
    li r4, 0x0
    bl fn_80102868
    mr r3, r31
    li r4, 0xe8c
    bl fn_801046C8
    stw r29, 0x4c(r3)
    mr r3, r31
    li r4, 0xe8d
    bl fn_801046C8
    stw r30, 0x4c(r3)
    mr r3, r28
    bl fn_800FA444
    clrlwi r0, r3, 16
    li r29, 0x0
    extsh r0, r0
    li r27, 0x0
    cmpwi r0, 0x32
    ble @L_8006DBB0
    li r29, -0x14
    li r27, -0xa
    @L_8006DBB0:
    mr r3, r31
    li r4, 0xe8a
    bl fn_801046C8
    stw r28, 0x4c(r3)
    li r4, 0xe87
    lha r0, 0x52(r3)
    add r0, r0, r29
    extsh r0, r0
    sth r0, 0x52(r3)
    mr r3, r31
    bl fn_801046C8
    neg r0, r28
    mr r30, r3
    or r0, r0, r28
    srwi r4, r0, 31
    bl fn_80109220
    lha r0, 0x52(r30)
    add r0, r0, r27
    extsh r0, r0
    sth r0, 0x52(r30)
    @L_8006DC00:
    lis r4, lbl_8026860C@ha
    mr r3, r31
    addi r4, r4, lbl_8026860C@l
    li r5, 0x8
    bl fn_80070D84
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
