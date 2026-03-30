    nofralloc
    stwu r1, -0xb0(r1)
    mflr r0
    stw r0, 0xb4(r1)
    stw r31, 0xac(r1)
    mr r31, r3
    stw r31, -23980(r13)
    lbz r0, 0x4(r3)
    cmplwi r0, 0x0
    beq @L_800D25EC
    lwz r3, 0xc(r31)
    bl fn_80194CC4
    mr r4, r3
    addi r3, r31, 0x94
    bl fn_800E0628
    lwz r3, 0xc(r31)
    addi r4, r31, 0x70
    bl fn_80195904
    lwz r3, 0xc(r31)
    addi r4, r31, 0xf4
    bl fn_801950D0
    lwz r3, 0xc(r31)
    addi r4, r31, 0x100
    bl fn_801959DC
    b @L_800D271C
@L_800D25EC:
    lbz r0, 0x1(r31)
    cmplwi r0, 0x1
    bne @L_800D269C
    addi r3, r1, 0x8
    addi r4, r31, 0x70
    addi r5, r31, 0x100
    bl fn_800E0168
    lfs f1, 0x8(r1)
    lfs f0, -27912(r2)
    fcmpo cr0, f1, f0
    ble @L_800D261C
    b @L_800D2620
@L_800D261C:
    fneg f1, f1
@L_800D2620:
    lis r3, -32696
    lfs f2, -30004(r3)
    fcmpo cr0, f1, f2
    bge @L_800D2684
    lfs f1, 0xc(r1)
    lfs f0, -27912(r2)
    fcmpo cr0, f1, f0
    ble @L_800D2644
    b @L_800D2648
@L_800D2644:
    fneg f1, f1
@L_800D2648:
    fcmpo cr0, f1, f2
    bge @L_800D2684
    lfs f1, 0x10(r1)
    lfs f0, -27912(r2)
    fcmpo cr0, f1, f0
    ble @L_800D2664
    b @L_800D2668
@L_800D2664:
    fneg f1, f1
@L_800D2668:
    fcmpo cr0, f1, f2
    bge @L_800D2684
    lfs f1, 0x100(r31)
    lfd f0, -27904(r2)
    fadd f0, f1, f0
    frsp f0, f0
    stfs f0, 0x100(r31)
@L_800D2684:
    addi r3, r31, 0x94
    addi r4, r31, 0x70
    addi r5, r31, 0xf4
    addi r6, r31, 0x100
    bl fn_800E0218
    b @L_800D271C
@L_800D269C:
    lfs f0, 0x88(r31)
    addi r3, r31, 0x94
    fneg f1, f0
    bl fn_800E053C
    lfs f0, 0x8c(r31)
    addi r3, r1, 0x44
    fneg f1, f0
    bl fn_800E0518
    lfs f0, 0x90(r31)
    addi r3, r1, 0x74
    fneg f1, f0
    bl fn_800E04F4
    lfs f1, 0x70(r31)
    addi r3, r1, 0x14
    lfs f2, 0x74(r31)
    lfs f0, 0x78(r31)
    fneg f1, f1
    fneg f2, f2
    fneg f3, f0
    bl fn_800E05C0
    addi r3, r31, 0x94
    addi r5, r1, 0x44
    mr r4, r3
    bl fn_800E0290
    addi r3, r31, 0x94
    addi r5, r1, 0x74
    mr r4, r3
    bl fn_800E0290
    addi r3, r31, 0x94
    addi r5, r1, 0x14
    mr r4, r3
    bl fn_800E0290
@L_800D271C:
    li r0, 0x0
    stb r0, 0x2(r31)
    lwz r0, 0xb4(r1)
    lwz r31, 0xac(r1)
    mtlr r0
    addi r1, r1, 0xb0
    blr
