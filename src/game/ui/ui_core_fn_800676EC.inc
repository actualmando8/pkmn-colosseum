    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r31, r3
    li r3, 0x1
    bl fn_800F7BC4
    rlwinm r0, r3, 0, 26, 26
    cmplwi r0, 0x0
    beq @L_800678E4
    li r3, 0x0
    bl fn_8025D2B0
    cmpwi r3, 0x1
    bne @L_80067948
    lis r3, lbl_803A9F08@ha
    addi r3, r3, lbl_803A9F08@l
    lwz r0, 0x0(r3)
    cmpwi r0, 0x2
    beq @L_80067848
    bge @L_80067754
    cmpwi r0, 0x0
    beq @L_8006775C
    bge @L_80067838
    b @L_80067948
@L_80067754:
    cmpwi r0, 0x4
    b @L_80067948
@L_8006775C:
    lbz r0, 0x4(r3)
    cmplwi r0, 0x0
    bne @L_80067828
    li r3, 0x1
    bl fn_800F7BC4
    mr r30, r3
    li r29, -0x1
    li r3, 0x0
    bl fn_8025D89C
    clrlwi r0, r30, 31
    clrlwi r3, r3, 16
    cmplwi r0, 0x0
    beq @L_80067794
    li r29, 0x0
@L_80067794:
    rlwinm r0, r30, 0, 28, 28
    cmplwi r0, 0x0
    beq @L_800677A4
    li r29, 0x1
@L_800677A4:
    rlwinm r0, r30, 0, 20, 20
    cmplwi r0, 0x0
    beq @L_800677B4
    li r29, 0x2
@L_800677B4:
    rlwinm r0, r30, 0, 29, 29
    cmplwi r0, 0x0
    beq @L_800677C4
    li r29, 0x3
@L_800677C4:
    rlwinm r0, r30, 0, 30, 30
    cmplwi r0, 0x0
    beq @L_800677D4
    li r29, 0x4
@L_800677D4:
    rlwinm r0, r30, 0, 21, 21
    cmplwi r0, 0x0
    beq @L_800677E4
    li r29, 0x5
@L_800677E4:
    cmpw r3, r29
    bgt @L_800677F0
    li r29, -0x1
@L_800677F0:
    cmpwi r29, 0x0
    blt @L_80067828
    li r3, 0x0
    bl fn_8025D89C
    clrlwi r0, r3, 16
    cmpw r29, r0
    bge @L_80067828
    li r0, 0x0
    lis r3, lbl_803A9F08@ha
    stb r0, 0x95(r31)
    li r0, 0x1
    addi r3, r3, lbl_803A9F08@l
    stb r0, 0x98(r31)
    stw r29, 0xc(r3)
@L_80067828:
    mr r3, r31
    li r4, 0x1
    bl fn_800679C0
    b @L_80067948
@L_80067838:
    mr r3, r31
    li r4, 0x1
    bl fn_800679C0
    b @L_80067948
@L_80067848:
    li r3, 0x1
    bl fn_800F7BC4
    lis r4, lbl_803A9F08@ha
    li r5, 0x0
    addi r4, r4, lbl_803A9F08@l
    lwz r0, 0xc(r4)
    cmpwi r0, 0x3
    beq @L_800678AC
    bge @L_80067884
    cmpwi r0, 0x1
    beq @L_8006789C
    bge @L_800678A4
    cmpwi r0, 0x0
    bge @L_80067894
    b @L_800678C0
@L_80067884:
    cmpwi r0, 0x5
    beq @L_800678BC
    bge @L_800678C0
    b @L_800678B4
@L_80067894:
    li r5, 0x1
    b @L_800678C0
@L_8006789C:
    li r5, 0x8
    b @L_800678C0
@L_800678A4:
    li r5, 0x800
    b @L_800678C0
@L_800678AC:
    li r5, 0x4
    b @L_800678C0
@L_800678B4:
    li r5, 0x2
    b @L_800678C0
@L_800678BC:
    li r5, 0x400
@L_800678C0:
    and r0, r3, r5
    cmplwi r0, 0x0
    bne @L_800678D4
    li r0, 0x1
    stb r0, 0x98(r31)
@L_800678D4:
    mr r3, r31
    li r4, 0x1
    bl fn_800679C0
    b @L_80067948
@L_800678E4:
    lis r3, lbl_803A9F08@ha
    addi r3, r3, lbl_803A9F08@l
    lwz r0, 0x0(r3)
    cmpwi r0, 0x2
    beq @L_80067934
    bge @L_8006790C
    cmpwi r0, 0x0
    beq @L_80067914
    bge @L_80067924
    b @L_80067948
@L_8006790C:
    cmpwi r0, 0x4
    b @L_80067948
@L_80067914:
    mr r3, r31
    li r4, 0x0
    bl fn_800679C0
    b @L_80067948
@L_80067924:
    mr r3, r31
    li r4, 0x1
    bl fn_800679C0
    b @L_80067948
@L_80067934:
    mr r3, r31
    li r4, 0x1
    bl fn_800679C0
    li r0, 0x1
    stb r0, 0x98(r31)
@L_80067948:
    bl fn_8006905C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80067964
    li r0, 0x1
    stb r0, 0x98(r31)
    stb r0, 0x99(r31)
@L_80067964:
    lis r3, lbl_803A9F08@ha
    addi r3, r3, lbl_803A9F08@l
    addis r3, r3, 0x1
    lbz r0, -0x31a8(r3)
    cmplwi r0, 0x0
    bne @L_80067988
    li r0, 0x1
    stb r0, 0x98(r31)
    stb r0, 0x99(r31)
@L_80067988:
    lis r3, lbl_803A9F08@ha
    addi r3, r3, lbl_803A9F08@l
    lwz r0, 0x0(r3)
    cmpwi r0, 0x1
    bne @L_800679A4
    mr r3, r31
    bl fn_80102ED4
@L_800679A4:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
