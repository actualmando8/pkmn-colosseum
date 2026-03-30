    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    li r28, 0x0
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    li r29, 0x0
    mr r31, r3
    b @L_801CB048
@L_801CB00C:
    mr r3, r31
    mr r4, r29
    bl fn_8012AC08
    mr r30, r3
    bl fn_80123FBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801CB044
    mr r3, r30
    bl fn_8011ED68
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801CB044
    addi r28, r28, 0x1
@L_801CB044:
    addi r29, r29, 0x1
@L_801CB048:
    rlwinm r0, r29, 0, 16, 31
    cmplwi r0, 0x6
    blt @L_801CB00C
    rlwinm r3, r28, 0, 16, 31
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
