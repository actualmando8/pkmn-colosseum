    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    mr r31, r4
    lha r0, 0x6(r31)
    cmpwi r0, 0x12ae
    beq @L_8000DC70
    bge @L_8000DB24
    cmpwi r0, 0x12ac
    beq @L_8000DC70
    bge @L_8000DB30
    b @L_8000DC70
    @L_8000DB24:
    cmpwi r0, 0x12b0
    bge @L_8000DC70
    b @L_8000DC34
    @L_8000DB30:
    li r4, 0x0
    bl fn_801040D0
    lis r4, 0x8889
    lis r6, 0x6666
    subi r0, r4, 0x7777
    lis r5, lbl_802E4B98@ha
    mulhw r8, r0, r3
    lis r4, lbl_803A1B80@ha
    addi r7, r6, 0x6667
    addi r4, r4, lbl_803A1B80@l
    li r6, 0x3a
    li r0, 0x0
    add r8, r8, r3
    sth r6, 0x4(r4)
    srawi r9, r8, 5
    addi r5, r5, lbl_802E4B98@l
    srawi r6, r8, 5
    sth r0, 0xa(r4)
    srwi r8, r6, 31
    srwi r10, r9, 31
    add r6, r6, r8
    mulli r6, r6, 0x3c
    add r0, r9, r10
    mulhw r8, r7, r0
    subf r6, r6, r3
    li r3, 0x37
    srawi r10, r8, 2
    srawi r8, r8, 2
    srwi r11, r10, 31
    mulhw r7, r7, r6
    srwi r9, r8, 31
    add r10, r10, r11
    add r11, r8, r9
    slwi r8, r10, 1
    lhzx r12, r5, r8
    srawi r9, r7, 2
    sth r12, 0x0(r4)
    srawi r7, r7, 2
    srwi r8, r7, 31
    srwi r10, r9, 31
    add r7, r7, r8
    add r8, r9, r10
    mulli r11, r11, 0xa
    slwi r8, r8, 1
    lhzx r8, r5, r8
    mulli r7, r7, 0xa
    subf r9, r11, r0
    sth r8, 0x6(r4)
    subf r0, r7, r6
    slwi r6, r9, 1
    slwi r0, r0, 1
    lhzx r6, r5, r6
    lhzx r0, r5, r0
    sth r6, 0x2(r4)
    sth r0, 0x8(r4)
    bl fn_80132A38
    mr r3, r30
    mr r4, r31
    bl fn_8001D834
    mr r5, r3
    li r3, 0x0
    li r4, 0x0
    li r6, 0xcf
    bl fn_800FB680
    b @L_8000DC70
    @L_8000DC34:
    li r4, 0x0
    bl fn_801040D0
    mr r4, r3
    li r3, 0x34
    bl fn_80132A38
    mr r3, r30
    mr r4, r31
    bl fn_8001D834
    lha r5, 0x54(r31)
    mr r7, r3
    lha r6, 0x56(r31)
    li r3, 0x0
    li r4, 0x0
    li r8, 0xde
    bl fn_800FBB34
    @L_8000DC70:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
