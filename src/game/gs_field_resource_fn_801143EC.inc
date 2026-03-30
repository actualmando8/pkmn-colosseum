    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    mr r30, r4
    rlwinm r0, r30, 0, 1, 15
    ori r4, r0, 0x400
    bl fn_800F9318
    mr r31, r3
    bl fn_801195AC
    cmplwi r3, 0x0
    beq @L_8011443C
    lis r5, -32751
    mr r4, r29
    addi r6, r5, 20664
    mr r5, r30
    bl fn_800F9378
@L_8011443C:
    mr r3, r31
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
