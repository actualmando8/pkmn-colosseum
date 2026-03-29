    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r29, r3
    mr r30, r4
    lwz r27, lbl_8047A4E0(r13)
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    cmplwi r27, 0x0
    lwz r0, 0x0(r3)
    beq @L_80041C78
    lwz r3, lbl_8047A4E4(r13)
    slwi r0, r0, 1
    lhzx r31, r3, r0
    cmplwi r31, 0x8000
    blt @L_80041C18
    clrlwi r31, r31, 18
    @L_80041C18:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r27
    clrlwi r4, r31, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r31
    li r3, 0x0
    bl fn_8025FDDC
    mr r28, r3
    mr r4, r31
    li r3, 0x0
    bl fn_8025FD34
    mr r31, r3
    mr r3, r27
    mr r4, r28
    bl fn_8011DFE0
    mr r3, r27
    mr r4, r31
    bl fn_8011DF90
    lwz r0, lbl_8047A4E0(r13)
    b @L_80041C7C
    @L_80041C78:
    li r0, 0x0
    @L_80041C7C:
    lis r3, lbl_803A6818@ha
    lfs f1, lbl_8047BCA0(r2)
    addi r3, r3, lbl_803A6818@l
    mr r31, r0
    lfs f0, 0x4c(r3)
    fmuls f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x8b(r29)
    lwz r0, 0x0(r3)
    lwz r3, lbl_8047A4E4(r13)
    slwi r0, r0, 1
    lhzx r0, r3, r0
    rlwinm r0, r0, 0, 16, 16
    cmpwi r0, 0x0
    beq @L_80041CC8
    li r0, 0x0
    b @L_80041CCC
    @L_80041CC8:
    li r0, 0x1
    @L_80041CCC:
    clrlwi r0, r0, 24
    cmplwi r0, 0x0
    beq @L_80041DC4
    mr r3, r31
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    clrlwi r4, r3, 16
    li r3, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_8012640C
    lis r4, lbl_802EF0A8@ha
    clrlwi r27, r3, 16
    addi r4, r4, lbl_802EF0A8@l
    lis r3, lbl_802E554C@ha
    lha r8, 0x50(r30)
    slwi r0, r27, 1
    lha r6, 0x5996(r4)
    addi r3, r3, lbl_802E554C@l
    lha r7, 0x52(r30)
    mr r5, r29
    lha r4, 0x5998(r4)
    subf r8, r8, r6
    lhzx r6, r3, r0
    extsh r3, r8
    subf r0, r7, r4
    li r7, 0x0
    extsh r4, r0
    bl fn_801040F0
    mr r3, r31
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    clrlwi r4, r3, 16
    li r3, 0x0
    li r5, 0x16
    li r6, 0x1
    bl fn_8012640C
    clrlwi r0, r3, 16
    cmplw r27, r0
    beq @L_80041E34
    lis r4, lbl_802EF0A8@ha
    lis r3, lbl_802E554C@ha
    addi r4, r4, lbl_802EF0A8@l
    lha r6, 0x50(r30)
    lha r5, 0x59b2(r4)
    slwi r0, r0, 1
    addi r3, r3, lbl_802E554C@l
    lha r7, 0x52(r30)
    lha r4, 0x59b4(r4)
    subf r8, r6, r5
    lhzx r6, r3, r0
    mr r5, r29
    subf r0, r7, r4
    extsh r3, r8
    extsh r4, r0
    li r7, 0x0
    bl fn_801040F0
    b @L_80041E34
    @L_80041DC4:
    lis r3, lbl_802EF0A8@ha
    lha r7, 0x50(r30)
    addi r6, r3, lbl_802EF0A8@l
    lha r3, 0x52(r30)
    lha r4, 0x5996(r6)
    mr r5, r29
    lha r0, 0x5998(r6)
    li r6, 0x5d
    subf r4, r7, r4
    li r7, 0x0
    subf r0, r3, r0
    extsh r3, r4
    extsh r4, r0
    bl fn_801040F0
    lis r3, lbl_802EF0A8@ha
    lha r7, 0x50(r30)
    addi r6, r3, lbl_802EF0A8@l
    lha r3, 0x52(r30)
    lha r4, 0x59b2(r6)
    mr r5, r29
    lha r0, 0x59b4(r6)
    li r6, 0x5d
    subf r4, r7, r4
    li r7, 0x0
    subf r0, r3, r0
    extsh r3, r4
    extsh r4, r0
    bl fn_801040F0
    @L_80041E34:
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
