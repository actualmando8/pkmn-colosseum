    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r29, r4
    mr r30, r5
    mr r31, r6
    mr r28, r3
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x32
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023CB28
    mr r4, r30
    li r3, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_8011BEB4
    cmplwi r3, 0x0
    bne @L_8023CB30
    lis r3, -32731
    addi r3, r3, -6868
    b @L_8023CB30
@L_8023CB28:
    li r3, 0x0
    b @L_8023CB4C
@L_8023CB30:
    mr r12, r3
    mr r3, r28
    mr r4, r29
    mr r5, r30
    mr r6, r31
    mtctr r12
    bctrl
@L_8023CB4C:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
