    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    bl fn_801F025C
    li r4, 0x0
    mr r31, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r31
    mr r31, r0
    bl fn_80203E0C
    rlwinm r30, r3, 0, 24, 31
    bl fn_800E0C54
    rlwinm r9, r3, 0, 16, 31
    li r8, 0xb
    divw r7, r9, r8
    li r0, 0x64
    mr r3, r31
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    mullw r7, r7, r8
    subf r7, r7, r9
    mulli r7, r7, 0xa
    addi r7, r7, 0x32
    mullw r7, r30, r7
    divw r7, r7, r0
    bl fn_8011BBD8
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
