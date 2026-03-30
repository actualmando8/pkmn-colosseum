    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r5, 0xd6
    li r6, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r3
    mr r3, r4
    li r4, 0x0
    bl fn_8012640C
    mr r0, r3
    li r3, 0x0
    mr r31, r0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r30
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r30, r3, 0, 16, 31
    mr r3, r31
    bl fn_80205BE8
    li r4, 0x0
    li r5, 0xc9
    li r6, 0x0
    bl fn_8012640C
    rlwinm r31, r3, 0, 16, 31
    mr r4, r30
    li r3, 0x0
    li r5, 0x33
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80236D48
    mr r4, r31
    li r3, 0x0
    li r5, 0x1d
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r3, r3, 0, 24, 31
    b @L_80236D4C
@L_80236D48:
    li r3, 0x1
@L_80236D4C:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
