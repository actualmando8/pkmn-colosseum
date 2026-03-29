    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    lis r4, lbl_803A3E58@ha
    li r3, 0x85
    addi r31, r4, lbl_803A3E58@l
    li r4, 0x0
    bl fn_8010264C
    lfs f1, lbl_8047BA4C(r2)
    li r3, 0x2
    bl fn_801C41C8
    lwz r0, lbl_8047A464(r13)
    cmpwi r0, 0x1
    beq @L_8003671C
    lis r3, fn_8003708C@ha
    addi r6, r31, 0x318
    addi r4, r3, fn_8003708C@l
    li r5, 0x0
    addi r3, r31, 0x0
    addi r6, r6, 0xffc
    li r7, 0x1000
    li r8, 0x10
    li r9, 0x1
    bl fn_800A19CC
    addi r3, r31, 0x0
    bl fn_800A1F94
    b @L_8003671C
    @L_80036718:
    bl fn_800F0308
    @L_8003671C:
    lwz r0, lbl_8047A464(r13)
    cmpwi r0, 0x0
    beq @L_80036718
    li r3, 0xa
    bl fn_8017B370
    lis r3, 0x8000
    li r4, 0x0
    addi r3, r3, 0x1801
    li r5, 0x17ff
    bl memset
    bl fn_8003686C
    lwz r3, lbl_8047A460(r13)
    li r7, 0x2
    addi r6, r31, 0x1640
    stw r7, 0x1318(r31)
    addi r8, r3, 0x1
    addi r5, r31, 0x1318
    li r0, 0x0
    lis r3, fn_800370E0@ha
    stw r8, lbl_8047A460(r13)
    addi r4, r3, fn_800370E0@l
    addi r3, r31, 0x1328
    addi r6, r6, 0xffc
    stw r0, 0x4(r5)
    li r7, 0x1000
    li r8, 0x10
    li r9, 0x1
    stw r0, 0x8(r5)
    stw r0, 0xc(r5)
    bl fn_800A19CC
    addi r3, r31, 0x1328
    bl fn_800A1F94
    lfs f1, lbl_8047BA4C(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    li r3, 0x85
    bl fn_80102510
    lwz r0, lbl_804788B8(r13)
    cmpwi r0, -0x1
    beq @L_8003684C
    bl fn_800D37CC
    xoris r3, r3, 0x8000
    lis r0, 0x4330
    stw r3, 0xc(r1)
    lfd f2, lbl_8047BA38(r2)
    stw r0, 0x8(r1)
    lfs f0, lbl_8047BA50(r2)
    lfd f1, 0x8(r1)
    fsubs f1, f1, f2
    fmuls f1, f0, f1
    bl fn_800C46B0
    clrlwi r31, r3, 16
    lwz r3, lbl_804788B8(r13)
    bl fn_800A0FC8
    lwz r0, lbl_804788B8(r13)
    cmplwi r0, 0x1
    bne @L_80036828
    mr r8, r31
    li r3, 0x1
    li r4, 0x2
    li r5, 0x0
    li r6, 0x2
    li r7, 0x1
    bl fn_800D37D4
    b @L_8003684C
    @L_80036828:
    mr r8, r31
    li r3, 0x1
    li r4, 0x2
    li r5, 0x0
    li r6, 0x2
    li r7, 0x0
    bl fn_800D37D4
    b @L_8003684C
    @L_80036848:
    bl fn_800F0308
    @L_8003684C:
    lwz r0, lbl_8047A460(r13)
    cmpwi r0, 0x0
    bne @L_80036848
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
