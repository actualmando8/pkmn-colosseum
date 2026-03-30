    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    li r29, 0x0
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    li r30, 0x0
    mr r31, r3
    b @L_801CB0D8
@L_801CB0AC:
    mr r3, r31
    mr r4, r30
    bl fn_8012AC08
    bl fn_80123FBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801CB0D4
    rlwinm r3, r29, 0, 16, 31
    addi r0, r3, 0x1
    rlwinm r29, r0, 0, 16, 31
@L_801CB0D4:
    addi r30, r30, 0x1
@L_801CB0D8:
    rlwinm r0, r30, 0, 16, 31
    cmplwi r0, 0x6
    blt @L_801CB0AC
    rlwinm r3, r29, 0, 16, 31
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
