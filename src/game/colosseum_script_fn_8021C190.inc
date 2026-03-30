    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    lwz r5, lbl_8047B610(r13)
    lwz r28, 0x1(r5)
    bl fn_801F025C
    li r4, 0x0
    mr r30, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r30
    mr r29, r0
    li r4, 0x2d
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021C1EC
    li r31, 0x0
    b @L_8021C1FC
@L_8021C1EC:
    mr r3, r30
    li r4, 0x2d
    bl fn_80202360
    mr r31, r3
@L_8021C1FC:
    extsh. r0, r31
    bgt @L_8021C218
    li r0, 0x0
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    stw r28, lbl_8047B610(r13)
    b @L_8021C2F4
@L_8021C218:
    mr r3, r30
    bl fn_80201704
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021C264
    mr r3, r30
    li r4, 0x2d
    bl fn_80202810
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
    li r0, 0x1
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    stw r28, lbl_8047B610(r13)
    b @L_8021C2F4
@L_8021C264:
    li r3, 0x2d
    bl fn_80119DD0
    rlwinm r0, r3, 0, 24, 31
    subf r0, r31, r0
    extsh. r0, r0
    bge @L_8021C280
    li r0, 0x0
@L_8021C280:
    extsh r0, r0
    li r3, 0x1
    slw r0, r3, r0
    mr r3, r30
    rlwinm r4, r0, 0, 16, 31
    bl fn_80203B5C
    rlwinm r0, r3, 0, 16, 31
    mr r3, r29
    neg r7, r0
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    lis r4, lbl_80379F58@ha
    mr r3, r30
    addi r4, r4, lbl_80379F58@l
    addis r5, r4, 0x1
    stb r31, 0x6002(r5)
    li r4, 0x2d
    bl fn_80202810
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
@L_8021C2F4:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
