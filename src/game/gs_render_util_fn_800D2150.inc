    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    rlwinm r0, r4, 0, 16, 31
    cmplwi r0, 0x27e
    ble @L_800D216C
    li r4, 0x27e
@L_800D216C:
    rlwinm r0, r5, 0, 16, 31
    cmplwi r0, 0x1de
    ble @L_800D217C
    li r5, 0x1de
@L_800D217C:
    rlwinm r0, r6, 0, 16, 31
    cmplwi r0, 0x27f
    ble @L_800D218C
    li r6, 0x27f
@L_800D218C:
    rlwinm r0, r7, 0, 16, 31
    cmplwi r0, 0x1df
    ble @L_800D219C
    li r7, 0x1df
@L_800D219C:
    addi r8, r6, 0x1
    addi r0, r7, 0x1
    mr r6, r5
    lwz r3, 0xc(r3)
    rlwinm r5, r8, 0, 16, 31
    rlwinm r7, r0, 0, 16, 31
    bl fn_801944A4
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
