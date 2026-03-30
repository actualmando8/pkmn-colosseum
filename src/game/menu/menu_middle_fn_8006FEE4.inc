    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r30, r3
    lbz r0, 0x95(r30)
    lis r3, lbl_80267EA8@ha
    addi r31, r3, lbl_80267EA8@l
    extsb r0, r0
    cmpwi r0, 0x6
    bge @L_8006FF14
    stw r0, lbl_8047A5FC(r13)
    @L_8006FF14:
    lwz r3, lbl_8047A5FC(r13)
    bl fn_8006B3C8
    mr r28, r3
    mr r3, r30
    li r4, 0x957
    bl fn_801046C8
    mr r25, r3
    cmplwi r25, 0x0
    beq @L_8006FF70
    clrlwi r0, r28, 24
    cmplwi r0, 0x0
    beq @L_8006FF58
    lwz r0, lbl_8047A5FC(r13)
    addi r3, r31, 0x218
    slwi r0, r0, 2
    lwzx r3, r3, r0
    b @L_8006FF5C
    @L_8006FF58:
    li r3, 0x26c
    @L_8006FF5C:
    bl fn_8005D858
    mr r0, r3
    mr r3, r25
    mr r4, r0
    bl fn_80071318
    @L_8006FF70:
    addi r27, r31, 0x1c8
    li r25, 0x0
    @L_8006FF78:
    lhz r4, 0x0(r27)
    mr r3, r30
    bl fn_801046C8
    mr r29, r3
    mr r3, r25
    bl fn_8006B3C8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8006FFA4
    lwz r0, 0x4(r27)
    b @L_8006FFA8
    @L_8006FFA4:
    li r0, 0x3daa
    @L_8006FFA8:
    stw r0, 0x4c(r29)
    addi r27, r27, 0x8
    addi r25, r25, 0x1
    cmpwi r25, 0x6
    blt @L_8006FF78
    slwi r0, r25, 3
    addi r27, r31, 0x1c8
    add r27, r27, r0
    b @L_8006FFE8
    @L_8006FFCC:
    lhz r4, 0x0(r27)
    mr r3, r30
    bl fn_801046C8
    lwz r0, 0x4(r27)
    addi r27, r27, 0x8
    addi r25, r25, 0x1
    stw r0, 0x4c(r3)
    @L_8006FFE8:
    cmplwi r25, 0xa
    blt @L_8006FFCC
    clrlwi r0, r28, 24
    addi r27, r31, 0x230
    cntlzw r0, r0
    li r25, 0x0
    srwi r29, r0, 5
    @L_80070004:
    lhz r4, 0x0(r27)
    mr r3, r30
    bl fn_801046C8
    clrlwi r4, r29, 24
    bl fn_80109220
    addi r27, r27, 0x2
    addi r25, r25, 0x1
    cmplwi r25, 0x5
    blt @L_80070004
    lbz r0, 0x1(r30)
    extsb r0, r0
    cmpwi r0, 0x3
    beq @L_800700AC
    bge @L_8007011C
    cmpwi r0, 0x0
    beq @L_80070048
    b @L_8007011C
    @L_80070048:
    addi r26, r31, 0x23c
    li r25, 0x0
    addi r29, r31, 0x0
    @L_80070054:
    srwi r0, r25, 31
    mr r27, r26
    add r0, r0, r25
    li r24, 0x0
    srawi r0, r0, 1
    slwi r3, r0, 2
    addi r28, r3, 0xc
    @L_80070070:
    lhz r4, 0x0(r27)
    mr r3, r30
    bl fn_801046C8
    lhzx r4, r29, r28
    addi r3, r3, 0xc
    bl fn_80108518
    addi r27, r27, 0x2
    addi r24, r24, 0x1
    cmplwi r24, 0x5
    blt @L_80070070
    addi r26, r26, 0xa
    addi r25, r25, 0x1
    cmplwi r25, 0x6
    blt @L_80070054
    b @L_8007011C
    @L_800700AC:
    lbz r0, 0x2(r30)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8007011C
    addi r28, r31, 0x23c
    li r25, 0x0
    addi r29, r31, 0x0
    @L_800700C8:
    srwi r0, r25, 31
    mr r26, r28
    add r0, r0, r25
    li r24, 0x0
    srawi r0, r0, 1
    slwi r3, r0, 2
    addi r27, r3, 0xe
    @L_800700E4:
    lhz r4, 0x0(r26)
    mr r3, r30
    bl fn_801046C8
    lhzx r4, r29, r27
    addi r3, r3, 0xc
    bl fn_80108518
    addi r26, r26, 0x2
    addi r24, r24, 0x1
    cmplwi r24, 0x5
    blt @L_800700E4
    addi r28, r28, 0xa
    addi r25, r25, 0x1
    cmplwi r25, 0x6
    blt @L_800700C8
    @L_8007011C:
    mr r3, r30
    addi r4, r31, 0x278
    li r5, 0xb
    bl fn_80070D84
    addi r26, r31, 0x2d0
    li r27, 0x0
    @L_80070134:
    cmpwi r27, 0x0
    lwz r3, lbl_8047A5FC(r13)
    bne @L_80070148
    li r4, 0x0
    b @L_8007014C
    @L_80070148:
    li r4, 0x1
    @L_8007014C:
    bl fn_8006B1F4
    lhz r4, 0x0(r26)
    mr r24, r3
    mr r3, r30
    bl fn_801046C8
    mr r4, r24
    bl fn_80109220
    lhz r4, 0x2(r26)
    mr r3, r30
    bl fn_801046C8
    mr r4, r24
    bl fn_80109220
    lhz r4, 0x4(r26)
    mr r3, r30
    bl fn_801046C8
    clrlwi r0, r24, 24
    cmplwi r0, 0x0
    beq @L_8007019C
    li r0, -0x1
    b @L_800701A4
    @L_8007019C:
    lis r4, 0x6060
    addi r0, r4, 0x60ff
    @L_800701A4:
    stw r0, 0x64(r3)
    addi r26, r26, 0x6
    addi r27, r27, 0x1
    cmplwi r27, 0x2
    blt @L_80070134
    li r27, 0x0
    la r26, lbl_8047C050(r2)
    @L_800701C0:
    lhz r4, 0x0(r26)
    mr r3, r30
    bl fn_801046C8
    srwi r4, r27, 31
    clrlwi r0, r27, 31
    xor r0, r0, r4
    mr r31, r3
    subf r0, r4, r0
    cmpwi r0, 0x0
    beq @L_80070210
    lfs f1, 0x70(r31)
    lfs f0, lbl_8047C078(r2)
    lfd f2, lbl_8047C080(r2)
    fadds f0, f1, f0
    stfs f0, 0x70(r31)
    lfs f1, 0x70(r31)
    bl fn_800CE318
    frsp f0, f1
    stfs f0, 0x70(r31)
    b @L_80070250
    @L_80070210:
    lfs f2, 0x70(r31)
    lfs f1, lbl_8047C078(r2)
    lfs f0, lbl_8047C088(r2)
    fsubs f1, f2, f1
    stfs f1, 0x70(r31)
    lfs f1, 0x70(r31)
    fcmpo cr0, f0, f1
    ble @L_8007023C
    lfs f0, lbl_8047C08C(r2)
    fadds f0, f1, f0
    stfs f0, 0x70(r31)
    @L_8007023C:
    lfs f1, 0x70(r31)
    lfd f2, lbl_8047C080(r2)
    bl fn_800CE318
    frsp f0, f1
    stfs f0, 0x70(r31)
    @L_80070250:
    addi r26, r26, 0x2
    addi r27, r27, 0x1
    cmplwi r27, 0x4
    blt @L_800701C0
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
