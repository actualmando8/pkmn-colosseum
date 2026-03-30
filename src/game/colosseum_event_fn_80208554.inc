    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r28, r4
    mr r29, r5
    mr r30, r6
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r31, r3
    beq @L_802085B0
@L_80208588:
    mr r3, r31
    mr r4, r28
    mr r5, r29
    mr r6, r30
    bl fn_801DA698
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_802085B0
    bl fn_800F0308
    b @L_80208588
@L_802085B0:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
