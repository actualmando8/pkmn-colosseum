    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    bne @L_8019C1CC
    lis r3, lbl_804653A8@ha
    addi r3, r3, lbl_804653A8@l
    @L_8019C1CC:
    lis r5, 0x4470
    li r6, 0x0
    subi r5, r5, 0x79a9
    mulhwu r5, r5, r4
    subf r0, r5, r4
    srwi r0, r0, 1
    add r0, r0, r5
    srwi r0, r0, 6
    mulli r0, r0, 0x65
    subf r0, r0, r4
    mr r7, r0
    slwi r0, r0, 2
    lwzx r5, r3, r0
    b @L_8019C24C
    @L_8019C204:
    lwz r0, 0x4(r5)
    cmplw r0, r4
    bne @L_8019C244
    cmplwi r6, 0x0
    beq @L_8019C224
    lwz r0, 0x0(r5)
    stw r0, 0x0(r6)
    b @L_8019C230
    @L_8019C224:
    lwz r4, 0x0(r5)
    slwi r0, r7, 2
    stwx r4, r3, r0
    @L_8019C230:
    lis r3, lbl_8046553C@ha
    mr r4, r5
    addi r3, r3, lbl_8046553C@l
    bl fn_801AA498
    b @L_8019C254
    @L_8019C244:
    mr r6, r5
    lwz r5, 0x0(r5)
    @L_8019C24C:
    cmplwi r5, 0x0
    bne @L_8019C204
    @L_8019C254:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
