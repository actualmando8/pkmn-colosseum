    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    bl fn_801F025C
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r30, r3
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x64
    divw r0, r4, r3
    mullw r0, r0, r3
    subf r0, r0, r4
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x5
    bge @L_80217844
    li r0, 0xa
    li r31, 0x4
    b @L_802178B0
@L_80217844:
    cmplwi r0, 0xf
    bge @L_80217858
    li r0, 0x1e
    li r31, 0x5
    b @L_802178B0
@L_80217858:
    cmplwi r0, 0x23
    bge @L_8021786C
    li r0, 0x32
    li r31, 0x6
    b @L_802178B0
@L_8021786C:
    cmplwi r0, 0x41
    bge @L_80217880
    li r0, 0x46
    li r31, 0x7
    b @L_802178B0
@L_80217880:
    cmplwi r0, 0x55
    bge @L_80217894
    li r0, 0x5a
    li r31, 0x8
    b @L_802178B0
@L_80217894:
    cmplwi r0, 0x5f
    bge @L_802178A8
    li r0, 0x6e
    li r31, 0x9
    b @L_802178B0
@L_802178A8:
    li r0, 0x96
    li r31, 0xa
@L_802178B0:
    mr r3, r30
    rlwinm r7, r0, 0, 16, 31
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_8011BBD8
    rlwinm r4, r31, 0, 24, 31
    li r3, 0x2f
    bl fn_80132A38
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
