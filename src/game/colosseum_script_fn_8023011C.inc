    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x120
    stw r0, 0x14(r1)
    li r6, 0x0
    stw r31, 0xc(r1)
    mr r31, r3
    bl fn_8012640C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80230190
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x46
    li r6, 0x0
    bl fn_801F4C14
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x42
    li r6, 0x0
    bl fn_801F4C14
    lis r4, lbl_80378721@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80378721@l
    li r5, 0x0
    bl fn_80211B94
@L_80230190:
    lwz r0, 0x14(r1)
    li r3, 0x1
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
