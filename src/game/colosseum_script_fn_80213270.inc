    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    bl fn_80261B68
    li r3, 0x0
    bl fn_80261E7C
    bl fn_8026246C
    li r5, 0x0
    li r4, 0x0
    la r3, lbl_80478D78(r13)
    b @L_802132B0
@L_802132A4:
    rlwinm r0, r5, 0, 24, 31
    addi r5, r5, 0x1
    stbx r4, r3, r0
@L_802132B0:
    rlwinm r0, r5, 0, 24, 31
    cmplwi r0, 0x8
    blt @L_802132A4
    lis r4, -32735
    li r3, 0x0
    addi r4, r4, 13524
    li r5, 0x0
    li r6, 0x0
    bl fn_801F37B0
    li r3, 0x0
    bl fn_801F27D4
    li r0, 0x1
    lis r3, -32735
    stb r0, 0x9(r1)
    addi r4, r3, 13656
    addi r5, r1, 0x9
    li r3, 0x0
    li r6, 0x0
    bl fn_801F37B0
    li r31, 0x0
    b @L_80213324
@L_80213304:
    mr r4, r31
    li r3, 0x0
    bl fn_801F47B4
    cmplwi r3, 0x0
    beq @L_80213320
    li r4, 0x4d
    bl fn_801F6EEC
@L_80213320:
    addi r31, r31, 0x1
@L_80213324:
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x2
    blt @L_80213304
    bl fn_802317E4
    lis r4, -32733
    li r3, 0x0
    addi r4, r4, 1384
    li r5, 0x0
    li r6, 0x1
    bl fn_801F37B0
    bl fn_8022FE80
    lis r4, -32733
    li r3, 0x0
    addi r4, r4, 792
    li r5, 0x0
    li r6, 0x0
    bl fn_801F37B0
    bl fn_801DA7AC
    lis r4, -32733
    li r3, 0x0
    addi r4, r4, 424
    li r5, 0x0
    li r6, 0x1
    bl fn_801F37B0
    bl fn_801DA7AC
    li r0, 0x0
    lis r3, -32735
    stb r0, 0x8(r1)
    addi r4, r3, 13656
    addi r5, r1, 0x8
    li r3, 0x0
    li r6, 0x0
    bl fn_801F37B0
    li r31, 0x0
    b @L_802133D0
@L_802133B0:
    mr r4, r31
    li r3, 0x0
    bl fn_801F47B4
    cmplwi r3, 0x0
    beq @L_802133CC
    li r4, 0x4d
    bl fn_801F6EEC
@L_802133CC:
    addi r31, r31, 0x1
@L_802133D0:
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x2
    blt @L_802133B0
    lwz r0, lbl_8047B618(r13)
    lis r3, lbl_80379F58@ha
    addi r4, r3, lbl_80379F58@l
    li r5, 0x0
    rlwinm r0, r0, 0, 23, 21
    la r3, lbl_80478D78(r13)
    stw r0, lbl_8047B618(r13)
    rlwinm r0, r0, 0, 13, 11
    addis r4, r4, 0x1
    mr r7, r5
    stw r0, lbl_8047B618(r13)
    rlwinm r6, r0, 0, 10, 8
    rlwinm r0, r6, 0, 12, 10
    stw r6, lbl_8047B618(r13)
    stw r0, lbl_8047B618(r13)
    stb r5, 0x6002(r4)
    stb r5, 0x60a1(r4)
    b @L_80213430
@L_80213424:
    rlwinm r0, r7, 0, 24, 31
    addi r7, r7, 0x1
    stbx r5, r3, r0
@L_80213430:
    rlwinm r0, r7, 0, 24, 31
    cmplwi r0, 0x5
    blt @L_80213424
    bl fn_801EF634
    rlwinm. r0, r3, 0, 16, 31
    bne @L_802134C0
    li r3, 0x0
    li r4, 0x0
    li r5, 0xc
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    addi r3, r3, 0x1
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xff
    ble @L_80213474
    li r3, 0xff
@L_80213474:
    rlwinm r7, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0xc
    li r6, 0x0
    bl fn_801F4C14
    bl fn_800E0C54
    rlwinm r7, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x5b
    li r6, 0x0
    bl fn_801F4C14
    bl fn_801DA7AC
    li r3, 0x0
    bl fn_80261B68
    li r3, 0x0
    bl fn_80261E7C
    bl fn_8026246C
@L_802134C0:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
