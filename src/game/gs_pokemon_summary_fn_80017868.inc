    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    lwz r3, lbl_8047A2F8(r13)
    li r4, 0xc
    li r5, 0x0
    bl fn_8012A5B0
    lwz r0, lbl_8047A2E0(r13)
    cmpwi r0, 0x3
    beq @L_800178A0
    li r3, 0x0
    b @L_800178D8
    @L_800178A0:
    mr r4, r3
    li r3, 0x50
    bl fn_80132A38
    li r3, 0x151
    bl fn_800FA444
    srwi r3, r3, 16
    lha r0, 0x54(r31)
    extsh r3, r3
    li r4, 0x0
    subf r3, r3, r0
    li r5, -0x1
    li r6, 0x151
    bl fn_800FB680
    li r3, 0x0
    @L_800178D8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
