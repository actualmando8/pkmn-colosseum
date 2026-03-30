    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r5, 0x43
    li r6, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    li r4, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r31
    bl fn_80229B70
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802393F4
    li r3, 0x64
    b @L_80239428
@L_802393F4:
    mr r3, r31
    bl fn_80229BD8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80239410
    li r3, 0x64
    b @L_80239428
@L_80239410:
    mr r4, r31
    li r3, 0x0
    li r5, 0x6
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r3, r3, 0, 24, 31
@L_80239428:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
