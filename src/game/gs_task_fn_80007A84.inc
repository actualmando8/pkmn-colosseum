    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    bl fn_801EF63C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80007AB0
    li r3, -0x1
    b @L_80007B18
    @L_80007AB0:
    lis r3, fn_800083FC@ha
    li r31, 0x162
    addi r30, r3, fn_800083FC@l
    @L_80007ABC:
    lhz r3, lbl_8047882A(r13)
    mr r5, r30
    addi r4, r1, 0x8
    bl fn_8001E304
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80007AE0
    li r0, -0x1
    b @L_80007AF8
    @L_80007AE0:
    lwz r0, 0x8(r1)
    cmplwi r0, 0x163
    blt @L_80007AF0
    stw r31, 0x8(r1)
    @L_80007AF0:
    bl fn_8001E200
    lwz r0, 0x8(r1)
    @L_80007AF8:
    cmpwi r0, 0x0
    bge @L_80007B08
    li r3, 0x1
    b @L_80007B18
    @L_80007B08:
    clrlwi r3, r0, 16
    sth r3, lbl_8047882A(r13)
    bl fn_80053110
    b @L_80007ABC
    @L_80007B18:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
