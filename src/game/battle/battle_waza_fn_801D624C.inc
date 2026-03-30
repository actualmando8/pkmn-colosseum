    nofralloc
    stwu r1, -0xd0(r1)
    mflr r0
    lis r3, -32698
    stw r0, 0xd4(r1)
    stmw r25, 0xb4(r1)
    addi r31, r3, 29688
    addi r30, r31, 0x61c
    lwz r3, 0x624(r31)
    lwz r0, 0x628(r31)
    rlwinm r3, r3, 2, 0, 29
    cmpwi r0, 0x0
    lwzx r3, r30, r3
    beq @L_801D697C
    mr r25, r31
    li r26, 0x0
    li r29, 0x0
@L_801D628C:
    lwz r3, 0x61c(r25)
    cmplwi r3, 0x0
    beq @L_801D62A0
    bl fn_801DB100
    stw r29, 0x61c(r25)
@L_801D62A0:
    addi r26, r26, 0x1
    addi r25, r25, 0x4
    cmpwi r26, 0x2
    blt @L_801D628C
    lwz r0, 0x63c(r31)
    cmpwi r0, 0x0
    beq @L_801D63CC
    lwz r28, 0x630(r31)
    lwz r27, 0x640(r31)
    lwz r29, 0x634(r31)
    rlwinm r3, r28, 0, 24, 31
    bl fn_801C3A64
    lfs f2, -21436(r2)
    fneg f1, f1
    addi r3, r1, 0x98
    fmr f3, f2
    bl fn_800E01F4
    lfs f1, -21436(r2)
    addi r3, r1, 0xa4
    lfs f2, -21428(r2)
    fmr f3, f1
    bl fn_800E01F4
    li r0, 0x0
    cmpwi r29, 0x0
    stw r0, 0x0(r30)
    beq @L_801D637C
    cmpwi r28, 0x0
    beq @L_801D6338
    li r3, -0x1
    bl fn_800E0C04
    neg r0, r27
    mr r4, r3
    or r0, r0, r27
    rlwinm r3, r29, 0, 16, 31
    rlwinm r5, r0, 1, 31, 31
    bl fn_801DE190
    mr r29, r3
    b @L_801D6344
@L_801D6338:
    rlwinm r3, r29, 0, 16, 31
    bl fn_801DE418
    mr r29, r3
@L_801D6344:
    cmplwi r29, 0x0
    beq @L_801D637C
    lwz r3, 0x24(r29)
    addi r4, r1, 0x98
    bl fn_800E43A4
    lwz r3, 0x24(r29)
    addi r4, r1, 0xa4
    bl fn_800E4170
    mr r3, r29
    li r4, 0x1
    bl fn_801DA4E8
    li r0, -0x1
    stb r0, 0x76(r29)
    stw r29, 0x0(r30)
@L_801D637C:
    li r3, 0x0
    stw r3, 0x628(r31)
    rlwinm r0, r3, 2, 0, 29
    stw r3, 0x62c(r31)
    stw r3, 0x624(r31)
    lwzx r4, r30, r0
    cmplwi r4, 0x0
    beq @L_801D6A50
    lwz r3, 0x0(r4)
    lwz r4, 0x4(r4)
    bl fn_801779CC
    li r3, 0x5
    bl fn_80177A44
    lfs f1, -21392(r2)
    bl fn_801767E0
    lfs f1, -21388(r2)
    bl fn_80176868
    lfs f1, -21436(r2)
    bl fn_80176758
    b @L_801D6A50
@L_801D63CC:
    lwz r0, 0x658(r31)
    cmpwi r0, 0x1
    beq @L_801D65A4
    bge @L_801D63E0
    b @L_801D6760
@L_801D63E0:
    cmpwi r0, 0x3
    bge @L_801D6760
    lwz r29, 0x634(r31)
    cmpwi r29, 0x0
    beq @L_801D64D4
    lwz r27, 0x630(r31)
    lwz r25, 0x654(r31)
    lwz r26, 0x650(r31)
    rlwinm r3, r27, 0, 24, 31
    lwz r28, 0x640(r31)
    bl fn_801C3A64
    lfs f2, -21436(r2)
    fneg f1, f1
    addi r3, r1, 0x80
    fmr f3, f2
    bl fn_800E01F4
    lfs f1, -21436(r2)
    addi r3, r1, 0x8c
    lfs f2, -21428(r2)
    fmr f3, f1
    bl fn_800E01F4
    li r0, 0x0
    cmpwi r29, 0x0
    stw r0, 0x0(r30)
    beq @L_801D64D4
    cmpwi r27, 0x0
    beq @L_801D6474
    li r3, -0x1
    bl fn_800E0C04
    neg r0, r28
    mr r4, r3
    or r0, r0, r28
    rlwinm r3, r29, 0, 16, 31
    rlwinm r5, r0, 1, 31, 31
    bl fn_801DE190
    mr r29, r3
    b @L_801D6480
@L_801D6474:
    rlwinm r3, r29, 0, 16, 31
    bl fn_801DE418
    mr r29, r3
@L_801D6480:
    cmplwi r29, 0x0
    beq @L_801D64D4
    lwz r3, 0x24(r29)
    addi r4, r1, 0x80
    bl fn_800E43A4
    lwz r3, 0x24(r29)
    addi r4, r1, 0x8c
    bl fn_800E4170
    mr r3, r29
    li r4, 0x1
    bl fn_801DA4E8
    li r0, -0x1
    cmpwi r26, 0x0
    stb r0, 0x76(r29)
    beq @L_801D64D0
    mr r3, r29
    rlwinm r4, r26, 0, 16, 31
    rlwinm r6, r25, 0, 24, 31
    li r5, 0x4
    bl fn_801DDD28
@L_801D64D0:
    stw r29, 0x0(r30)
@L_801D64D4:
    lwz r27, 0x648(r31)
    cmpwi r27, 0x0
    beq @L_801D693C
    lwz r25, 0x644(r31)
    lwz r26, 0x64c(r31)
    rlwinm r3, r25, 0, 24, 31
    bl fn_801C3A64
    lfs f2, -21436(r2)
    addi r3, r1, 0x68
    fmr f3, f2
    bl fn_800E01F4
    lfs f1, -21436(r2)
    addi r3, r1, 0x74
    lfs f2, -21432(r2)
    fmr f3, f1
    bl fn_800E01F4
    lis r3, -32698
    cmpwi r27, 0x0
    addi r30, r3, 29688
    li r0, 0x0
    stw r0, 0x620(r30)
    beq @L_801D693C
    cmpwi r25, 0x0
    beq @L_801D655C
    li r3, -0x1
    bl fn_800E0C04
    neg r0, r26
    mr r4, r3
    or r0, r0, r26
    rlwinm r3, r27, 0, 16, 31
    rlwinm r5, r0, 1, 31, 31
    bl fn_801DE190
    mr r29, r3
    b @L_801D6568
@L_801D655C:
    rlwinm r3, r27, 0, 16, 31
    bl fn_801DE418
    mr r29, r3
@L_801D6568:
    cmplwi r29, 0x0
    beq @L_801D693C
    lwz r3, 0x24(r29)
    addi r4, r1, 0x68
    bl fn_800E43A4
    lwz r3, 0x24(r29)
    addi r4, r1, 0x74
    bl fn_800E4170
    mr r3, r29
    li r4, 0x1
    bl fn_801DA4E8
    li r0, 0x1
    stb r0, 0x76(r29)
    stw r29, 0x620(r30)
    b @L_801D693C
@L_801D65A4:
    lwz r29, 0x634(r31)
    cmpwi r29, 0x0
    beq @L_801D6690
    lwz r25, 0x630(r31)
    lwz r28, 0x654(r31)
    lwz r27, 0x650(r31)
    rlwinm r3, r25, 0, 24, 31
    lwz r26, 0x640(r31)
    bl fn_801C3A64
    lfs f2, -21436(r2)
    fneg f1, f1
    addi r3, r1, 0x50
    fmr f3, f2
    bl fn_800E01F4
    lfs f1, -21436(r2)
    addi r3, r1, 0x5c
    lfs f2, -21428(r2)
    fmr f3, f1
    bl fn_800E01F4
    li r0, 0x0
    cmpwi r29, 0x0
    stw r0, 0x0(r30)
    beq @L_801D6690
    cmpwi r25, 0x0
    beq @L_801D6630
    li r3, -0x1
    bl fn_800E0C04
    neg r0, r26
    mr r4, r3
    or r0, r0, r26
    rlwinm r3, r29, 0, 16, 31
    rlwinm r5, r0, 1, 31, 31
    bl fn_801DE190
    mr r29, r3
    b @L_801D663C
@L_801D6630:
    rlwinm r3, r29, 0, 16, 31
    bl fn_801DE418
    mr r29, r3
@L_801D663C:
    cmplwi r29, 0x0
    beq @L_801D6690
    lwz r3, 0x24(r29)
    addi r4, r1, 0x50
    bl fn_800E43A4
    lwz r3, 0x24(r29)
    addi r4, r1, 0x5c
    bl fn_800E4170
    mr r3, r29
    li r4, 0x1
    bl fn_801DA4E8
    li r0, -0x1
    cmpwi r27, 0x0
    stb r0, 0x76(r29)
    beq @L_801D668C
    mr r3, r29
    rlwinm r4, r27, 0, 16, 31
    rlwinm r6, r28, 0, 24, 31
    li r5, 0x3
    bl fn_801DDD28
@L_801D668C:
    stw r29, 0x0(r30)
@L_801D6690:
    lwz r27, 0x648(r31)
    cmpwi r27, 0x0
    beq @L_801D693C
    lwz r25, 0x644(r31)
    lwz r26, 0x64c(r31)
    rlwinm r3, r25, 0, 24, 31
    bl fn_801C3A64
    lfs f2, -21436(r2)
    addi r3, r1, 0x38
    fmr f3, f2
    bl fn_800E01F4
    lfs f1, -21436(r2)
    addi r3, r1, 0x44
    lfs f2, -21432(r2)
    fmr f3, f1
    bl fn_800E01F4
    lis r3, -32698
    cmpwi r27, 0x0
    addi r30, r3, 29688
    li r0, 0x0
    stw r0, 0x620(r30)
    beq @L_801D693C
    cmpwi r25, 0x0
    beq @L_801D6718
    li r3, -0x1
    bl fn_800E0C04
    neg r0, r26
    mr r4, r3
    or r0, r0, r26
    rlwinm r3, r27, 0, 16, 31
    rlwinm r5, r0, 1, 31, 31
    bl fn_801DE190
    mr r29, r3
    b @L_801D6724
@L_801D6718:
    rlwinm r3, r27, 0, 16, 31
    bl fn_801DE418
    mr r29, r3
@L_801D6724:
    cmplwi r29, 0x0
    beq @L_801D693C
    lwz r3, 0x24(r29)
    addi r4, r1, 0x38
    bl fn_800E43A4
    lwz r3, 0x24(r29)
    addi r4, r1, 0x44
    bl fn_800E4170
    mr r3, r29
    li r4, 0x1
    bl fn_801DA4E8
    li r0, 0x1
    stb r0, 0x76(r29)
    stw r29, 0x620(r30)
    b @L_801D693C
@L_801D6760:
    lwz r29, 0x634(r31)
    cmpwi r29, 0x0
    beq @L_801D684C
    lwz r25, 0x630(r31)
    lwz r28, 0x654(r31)
    lwz r27, 0x650(r31)
    rlwinm r3, r25, 0, 24, 31
    lwz r26, 0x640(r31)
    bl fn_801C3A64
    lfs f2, -21436(r2)
    fneg f1, f1
    addi r3, r1, 0x20
    fmr f3, f2
    bl fn_800E01F4
    lfs f1, -21436(r2)
    addi r3, r1, 0x2c
    lfs f2, -21428(r2)
    fmr f3, f1
    bl fn_800E01F4
    li r0, 0x0
    cmpwi r29, 0x0
    stw r0, 0x0(r30)
    beq @L_801D684C
    cmpwi r25, 0x0
    beq @L_801D67EC
    li r3, -0x1
    bl fn_800E0C04
    neg r0, r26
    mr r4, r3
    or r0, r0, r26
    rlwinm r3, r29, 0, 16, 31
    rlwinm r5, r0, 1, 31, 31
    bl fn_801DE190
    mr r29, r3
    b @L_801D67F8
@L_801D67EC:
    rlwinm r3, r29, 0, 16, 31
    bl fn_801DE418
    mr r29, r3
@L_801D67F8:
    cmplwi r29, 0x0
    beq @L_801D684C
    lwz r3, 0x24(r29)
    addi r4, r1, 0x20
    bl fn_800E43A4
    lwz r3, 0x24(r29)
    addi r4, r1, 0x2c
    bl fn_800E4170
    mr r3, r29
    li r4, 0x1
    bl fn_801DA4E8
    li r0, -0x1
    cmpwi r27, 0x0
    stb r0, 0x76(r29)
    beq @L_801D6848
    mr r3, r29
    rlwinm r4, r27, 0, 16, 31
    rlwinm r6, r28, 0, 24, 31
    li r5, 0x1
    bl fn_801DDD28
@L_801D6848:
    stw r29, 0x0(r30)
@L_801D684C:
    lwz r30, 0x648(r31)
    cmpwi r30, 0x0
    beq @L_801D693C
    lwz r25, 0x644(r31)
    lwz r28, 0x654(r31)
    lwz r27, 0x650(r31)
    rlwinm r3, r25, 0, 24, 31
    lwz r26, 0x64c(r31)
    bl fn_801C3A64
    lfs f2, -21436(r2)
    addi r3, r1, 0x8
    fmr f3, f2
    bl fn_800E01F4
    lfs f1, -21436(r2)
    addi r3, r1, 0x14
    lfs f2, -21432(r2)
    fmr f3, f1
    bl fn_800E01F4
    lis r3, -32698
    cmpwi r30, 0x0
    addi r29, r3, 29688
    li r0, 0x0
    stw r0, 0x620(r29)
    beq @L_801D693C
    cmpwi r25, 0x0
    beq @L_801D68DC
    li r3, -0x1
    bl fn_800E0C04
    neg r0, r26
    mr r4, r3
    or r0, r0, r26
    rlwinm r3, r30, 0, 16, 31
    rlwinm r5, r0, 1, 31, 31
    bl fn_801DE190
    mr r30, r3
    b @L_801D68E8
@L_801D68DC:
    rlwinm r3, r30, 0, 16, 31
    bl fn_801DE418
    mr r30, r3
@L_801D68E8:
    cmplwi r30, 0x0
    beq @L_801D693C
    lwz r3, 0x24(r30)
    addi r4, r1, 0x8
    bl fn_800E43A4
    lwz r3, 0x24(r30)
    addi r4, r1, 0x14
    bl fn_800E4170
    mr r3, r30
    li r4, 0x1
    bl fn_801DA4E8
    li r0, 0x1
    cmpwi r27, 0x0
    stb r0, 0x76(r30)
    beq @L_801D6938
    mr r3, r30
    rlwinm r4, r27, 0, 16, 31
    rlwinm r6, r28, 0, 24, 31
    li r5, 0x2
    bl fn_801DDD28
@L_801D6938:
    stw r30, 0x620(r29)
@L_801D693C:
    li r3, 0x0
    stw r3, 0x628(r31)
    stw r3, 0x62c(r31)
    stw r3, 0x624(r31)
    lwz r0, 0x61c(r31)
    cmplwi r0, 0x0
    beq @L_801D6960
    stw r3, 0x624(r31)
    b @L_801D6A50
@L_801D6960:
    lwz r0, 0x620(r31)
    li r4, 0x1
    cmplwi r0, 0x0
    beq @L_801D6A50
    stw r4, 0x624(r31)
    b @L_801D6A50
    b @L_801D6A50
@L_801D697C:
    lwz r0, 0x62c(r31)
    cmpwi r0, 0x0
    beq @L_801D6A50
    bl fn_801DAC54
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801D6A50
    lwz r5, 0x624(r31)
    lis r3, -32698
    addi r4, r3, 29688
    addi r0, r5, 0x1
    stw r0, 0x624(r31)
    b @L_801D69FC
@L_801D69AC:
    rlwinm r0, r0, 2, 0, 29
    add r3, r4, r0
    lwz r25, 0x61c(r3)
    cmplwi r25, 0x0
    beq @L_801D69F0
    lwz r0, 0x68(r25)
    cmplwi r0, 0x0
    beq @L_801D6A08
    mr r3, r25
    li r4, 0x1
    bl fn_801DA4E8
    lwz r5, 0x68(r25)
    mr r3, r25
    lhz r4, 0x2c(r5)
    lhz r5, 0x2e(r5)
    bl fn_801DA9E8
    b @L_801D6A08
@L_801D69F0:
    lwz r3, 0x624(r31)
    addi r0, r3, 0x1
    stw r0, 0x624(r31)
@L_801D69FC:
    lwz r0, 0x624(r31)
    cmpwi r0, 0x2
    blt @L_801D69AC
@L_801D6A08:
    lwz r0, 0x624(r31)
    cmpwi r0, 0x2
    blt @L_801D6A50
    li r4, 0x0
    lis r3, -32698
    stw r4, 0x624(r31)
    addi r3, r3, 29688
    stw r4, 0x62c(r31)
    lwz r0, 0x61c(r3)
    cmplwi r0, 0x0
    beq @L_801D6A3C
    stw r4, 0x624(r31)
    b @L_801D6A50
@L_801D6A3C:
    lwz r0, 0x620(r3)
    li r4, 0x1
    cmplwi r0, 0x0
    beq @L_801D6A50
    stw r4, 0x624(r31)
@L_801D6A50:
    lmw r25, 0xb4(r1)
    lwz r0, 0xd4(r1)
    mtlr r0
    addi r1, r1, 0xd0
    blr
