    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    li r30, 0x0
    b @L_8003630C
    @L_8003625C:
    bl fn_8017B1AC
    cmpwi r3, 0xb
    beq @L_80036270
    cmpwi r3, 0x5
    bne @L_80036278
    @L_80036270:
    bl fn_800F0308
    b @L_8003630C
    @L_80036278:
    li r3, 0x1
    bl fn_800F7AF0
    mr r31, r3
    li r3, 0x1
    bl fn_800F7BC4
    and r0, r3, r31
    andi. r0, r0, 0x1300
    cmplwi r0, 0x0
    beq @L_800362A4
    bl fn_801E1810
    b @L_8003631C
    @L_800362A4:
    cmplwi r30, 0x21
    mr r31, r30
    blt @L_800362B4
    b @L_80036304
    @L_800362B4:
    bl fn_801E16D0
    cmpwi r3, 0x0
    bge @L_800362C4
    b @L_80036304
    @L_800362C4:
    mulli r5, r30, 0x6
    lis r4, lbl_802E50E0@ha
    addi r0, r4, lbl_802E50E0@l
    add r30, r0, r5
    lhz r0, 0x0(r30)
    cmpw r3, r0
    blt @L_80036304
    lhz r3, 0x2(r30)
    cmplwi r3, 0x0
    beq @L_800362F0
    bl fn_80166A28
    @L_800362F0:
    lhz r3, 0x4(r30)
    cmplwi r3, 0x0
    beq @L_80036300
    bl fn_80166A28
    @L_80036300:
    addi r31, r31, 0x1
    @L_80036304:
    mr r30, r31
    bl fn_800F0308
    @L_8003630C:
    bl fn_801E1874
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_8003625C
    @L_8003631C:
    li r3, 0x1
    li r4, 0x3e8
    li r5, 0x0
    li r6, 0x7f
    bl fn_8016597C
    li r3, 0x384
    bl fn_800FF58C
    lis r4, 0x596
    li r3, 0x0
    addi r4, r4, 0x8
    bl fn_8011288C
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
